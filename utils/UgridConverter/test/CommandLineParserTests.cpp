
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
#include "catch.hpp"
#include <nlohmann/json.hpp>
#include "../CommandLineParser.h"

TEST_CASE("Determine input filetype"){
    REQUIRE(determineInputType("grid.lb8.ugrid") == "lb8");
    REQUIRE(determineInputType("grid.b8.ugrid") == "b8");
    REQUIRE(determineInputType("grid.ugrid") == "ascii");
}

TEST_CASE("Determine output filetype"){
    REQUIRE(determineOutputType("grid.lb8.ugrid") == "lb8");
    REQUIRE(determineOutputType("grid.b8.ugrid") == "b8");
    REQUIRE(determineOutputType("grid.stl") == "stl");
    REQUIRE(determineOutputType("grid.vtk") == "vtk");
    REQUIRE(determineOutputType("grid.surface.vtk") == "vtk-surface");
    REQUIRE(determineOutputType("grid.exploded.vtk") == "vtk-exploded");
}

TEST_CASE("Command Line can get input file"){
    std::vector<std::string> command_line = {"-i","pokemon.b8.ugrid", "-o", "pikachu.lb8.ugrid"};
    auto config = getConfigFromCommandLine(command_line);
    std::string file_in = config["input"]["file"];
    std::string type_in = config["input"]["type"];
    REQUIRE("pokemon.b8.ugrid" == file_in);
    REQUIRE("b8" == type_in);

    std::string file_out = config["output"]["file"];
    std::string type_out = config["output"]["type"];
    REQUIRE("pikachu.lb8.ugrid" == file_out);
    REQUIRE("lb8" == type_out);
}
