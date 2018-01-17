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
    std::vector<std::string> command_line = {"-i","pokemon.lb8.ugrid", "-o", "pokemon.lb8.ugrid"};
    auto config = getConfigFromCommandLine(command_line);
    std::string file_in = config["input"]["file"];
    std::string type = config["input"]["type"];
    REQUIRE("pokemon.lb8.ugrid" == file_in);
    REQUIRE("lb8" == type);
}

