
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
#include "../../parfait/ImportedUgridFactory.h"
#include "../../parfait/UgridWriter.h"
#include <VtkUnstructuredWriter.h>
#include <VtkUnstructuredSurfaceWriter.h>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "CommandLineParser.h"
#include "../../parfait/ExplodedMesh.h"
#include "../../parfait/STL.h"

std::string stripExtension(std::string in){
    return in.substr(0,in.rfind(".ugrid"));
}

std::shared_ptr<ImportedUgrid> readInputGrid(std::string filename, std::string file_type){
    if(file_type == "ascii") {
        printf("\nReading file <%s> as ascii.\n", filename.c_str());
        return std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgridAscii(filename)
        );
    } else if (file_type == "lb8") {
        return std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgrid(filename));
    } else if (file_type == "b8") {
        return std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgrid(filename, true));
    } else {
        throw std::logic_error("\nInvalid file type for reading\n\n");
    }
}

void writeOutputFile(std::shared_ptr<ImportedUgrid> ugrid, std::string filename, std::string file_type){
    if(file_type == "lb8"){
        printf("Exporting <%s> as binary.", filename.c_str());
        Parfait::UgridWriter::writeImportedUgrid(*ugrid.get(), filename + ".lb8.ugrid", false);
    } else if(file_type == "vtk") {
        printf("Exporting <%s> as vtu.", filename.c_str());
        Parfait::VtkUnstructuredWriter writer(filename, *ugrid.get());
        writer.writeBinary();
    } else if(file_type == "vtk-surface"){
        printf("Exporting <%s> surfaces as vtu.", filename.c_str());
        Parfait::VtkUnstructuredSurfaceWriter writer(filename + ".surface", *ugrid.get());
        std::vector<int> bc_tag;
        for(auto t : ugrid->triangleTags)
            bc_tag.push_back(t);
        for(auto t : ugrid->quadTags)
            bc_tag.push_back(t);
        writer.addCellData("boundary-tag", bc_tag.data(), 1);
        writer.writeBinary();
    } else if(file_type == "vtk-exploded") {
        printf("Exporting <%s> as exploded vtu", filename.c_str());
        Parfait::ExplodedMesh explodedMesh(*ugrid.get());;
        Parfait::VtkUnstructuredWriter writer(filename + ".exploded", explodedMesh);
        writer.writeBinary();
    } else if(file_type == "stl") {
        Parfait::STL::STL stl;
        for(int t = 0; t < ugrid->triangles.size() / 3; t++){
            auto* tri = &ugrid->triangles[3*t];
            Parfait::Facet f = Parfait::Facet{ugrid->getPoint(tri[0]),
                                              ugrid->getPoint(tri[1]),
                                              ugrid->getPoint(tri[2])};
            f.normal[0] = ugrid->triangleTags[t];
            stl.facets.push_back(f);
        }
        for(int q = 0; q < ugrid->quads.size() / 4; q++){
            auto* quad = &ugrid->quads[4*q];
            Parfait::Facet f = {ugrid->getPoint(quad[0]),
                                ugrid->getPoint(quad[1]),
                                ugrid->getPoint(quad[2])};
            f.normal[0] = ugrid->quadTags[q];
            stl.facets.push_back(f);

            f = {ugrid->getPoint(quad[2]),
                 ugrid->getPoint(quad[3]),
                 ugrid->getPoint(quad[0])};
            f.normal[0] = ugrid->quadTags[q];
            stl.facets.push_back(f);
        }
        stl.writeAsciiFile(filename);
    } else {
        std::cout << "Request <" << file_type << "> is unknown" << std::endl;
    }
}


int main(int argc, char* argv[]) {

    std::vector<std::string> command_line;
    for(int i = 0; i < argc; i++){
        command_line.push_back(argv[i]);
    }

    auto config = getConfigFromCommandLine(command_line);

    std::string filename = config["input"]["file"];
    std::string input_type = config["input"]["type"];

    auto ugrid = readInputGrid(config["input"]["file"], config["input"]["type"]);

    auto base = stripExtension(filename);

    writeOutputFile(ugrid, config["output"]["file"], config["output"]["type"]);

    std::cout << std::endl;
    return 0;
}
