
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once
#include "../../parfait/StringTools.h"
#include <string>
#include <set>

inline std::set<std::string> inputTypes(){
    return {"lb8", "b8", "ascii"};
}

inline void printHelpAndExit(){
    printf("\n\nPrinting help and Exiting\n");
    printf("Input arguments:\n");
    printf("   -i <input filename>\n");
    printf("      options: [lb8.ugrid, b8.ugrid]\n");
    printf("Output arguments:\n");
    printf("   -o <output filename>\n");
    printf("      options: [.vtk, .surface.vtk, .exploded.vtk, lb8.ugrid, .stl ]\n");
    exit(0);
}

inline void throwIfConfigIsInvalid(nlohmann::json config){
    std::string sdum = config["input"]["file"];
    sdum = config["input"]["type"];
    sdum = config["output"]["file"];
    sdum = config["output"]["type"];
}



inline std::string determineInputType(std::string filename){
    auto filename_components = Parfait::StringTools::split(filename, ".");
    if(filename_components.back() != "ugrid") throw std::logic_error("Can only read ugrid files");
    filename_components.pop_back();
    if(filename_components.back() == "lb8" or filename_components.back() == "b8")
        return filename_components.back();
    else{
        printf("assuming file %s is an ascii file\n", filename.c_str());
        return "ascii";
    }
}

inline std::string determineOutputType(std::string filename){
    auto filename_components = Parfait::StringTools::split(filename, ".");
    if(filename_components.back() == "ugrid")
        filename_components.pop_back();
    if(filename_components.back() == "lb8" or filename_components.back() == "b8" or filename_components.back() == "stl")
        return filename_components.back();
    if(filename_components.back() == "vtk"){
        int back = filename_components.size()-1;
        int next = back-1;
        if(filename_components[next] == "surface")
            return "vtk-surface";
        else if(filename_components[next] == "exploded")
            return "vtk-exploded";
        else
            return "vtk";
    }
    else
        throw std::logic_error("Could not determine output file type");
}

inline nlohmann::json getConfigFromCommandLine(std::vector<std::string>& command_line){
    nlohmann::json config;
    for(int index = 0; index < command_line.size(); index++){
        if(command_line[index] == "-h" or command_line[index] == "--help"){
            printHelpAndExit();
        }
        if(command_line[index] == "-i"){
            config["input"]["file"] = command_line[++index];
            config["input"]["type"] = determineInputType(config["input"]["file"]);
        } else if(command_line[index] == "-o"){
            config["output"]["file"] = command_line[++index];
            config["output"]["type"] = determineOutputType(config["output"]["file"]);
        }
    }
    try{
        throwIfConfigIsInvalid(config);
    } catch(std::exception &e){
        printf("Missing element in config from command line: \n");
        for(auto s : command_line)
            printf("%s\n", s.c_str());
        printf("%s\n", e.what());
        printHelpAndExit();
    }
    return config;
}



