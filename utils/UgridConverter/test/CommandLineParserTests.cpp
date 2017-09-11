#include "catch.hpp"
#include <nlohmann/json.hpp>
#include "../CommandLineParser.h"


TEST_CASE("Command Line can get input file"){
    std::vector<std::string> command_line = {"-i","grid.ugrid","-it", "lb8"};
    auto config = getConfigFromCommandLine(command_line);
    std::string file_in = config["input"]["file"];
    std::string type = config["input"]["type"];
    REQUIRE("grid.ugrid" == file_in);
    REQUIRE("lb8" == type);
}

TEST_CASE("Command Line can get output file"){
    std::vector<std::string> command_line = {"-o","grid.ugrid","-ot", "lb8"};
    auto config = getConfigFromCommandLine(command_line);
    std::string file_in = config["output"]["file"];
    std::string type = config["output"]["type"];
    REQUIRE("grid.ugrid" == file_in);
    REQUIRE("lb8" == type);
}

TEST_CASE("Throw if input and out including types are missing"){
    std::vector<std::string> command_line = {"-o","grid.ugrid","-ot", "lb8"};
    auto config = getConfigFromCommandLine(command_line);
    REQUIRE_THROWS(throwIfConfigIsInvalid(config));
}
