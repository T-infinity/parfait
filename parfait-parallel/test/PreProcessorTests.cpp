#include <string>
#include <ParallelMeshReader.h>
#include "PreProcessor.h"
#include <catch.hpp>

using namespace Parfait;

TEST_CASE("Read in grid based on json file (on one proc)") {
    std::string input_json = SIX_CELL_TEST_JSON;
    ConfigurationReader reader(input_json);
    Configuration config = reader.createConfiguration();

    PreProcessor preProcessor(config);
    auto mesh = preProcessor.createMesh();

    if(MessagePasser::NumberOfProcesses() == 1){
        REQUIRE(12 == mesh->numberOfTriangles());
        REQUIRE(6 == mesh->numberOfQuads());
        REQUIRE(0 == mesh->numberOfTets());
        REQUIRE(0 == mesh->numberOfPyramids());
        REQUIRE(6 == mesh->numberOfPrisms());
        REQUIRE(0 == mesh->numberOfHexes());
        for(int i = 0; i < 14; i++){
            REQUIRE(i == mesh->getGlobalNodeId(i));
        }
        for(int i = 0; i < 14; i++){
            REQUIRE(not mesh->isGhostNode(i));
        }
    }

    if(MessagePasser::NumberOfProcesses() == 2){
        REQUIRE(mesh->numberOfNodes() == 14);
    }

    BoundaryConditionMap boundaryConditionMap = config.getMapbcObject(0);
}

TEST_CASE("Add Owning Rank test"){
    std::string input_json = SIX_CELL_TEST_JSON;
    ConfigurationReader reader(input_json);
    Configuration config = reader.createConfiguration();

    PreProcessor preProcessor(config);
    auto mesh = preProcessor.createMesh();

    if(MessagePasser::NumberOfProcesses() == 2){
        int other  = (MessagePasser::Rank() == 0)?(1):(0);
        for(int i = 0; i < 7; i++){
            REQUIRE(MessagePasser::Rank() == mesh->owningRankOfNode(i));
        }
        for(int i = 7; i < 14; i++){
            REQUIRE(other == mesh->owningRankOfNode(i));
        }
    }
}
