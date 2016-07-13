#include <string>
#include <ParallelMeshReader.h>
#include "PreProcessor.h"
#include <catch.hpp>

using std::string;
using namespace Parfait;

TEST_CASE("Read in grid based on xml file (on one proc)") {
    ConfigurationReader reader("6cell.xml");
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

    BoundaryConditionMap boundaryConditionMap = config.getMapbcObject(0);
}
