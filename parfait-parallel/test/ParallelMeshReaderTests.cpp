#include <string>
#include <ParallelMeshReader.h>
#include <catch.hpp>

using std::string;

TEST_CASE("Read in the grid and check it if run on one proc") {
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid({"../../grids/6cell.lb8.ugrid"}, {false});
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
}

TEST_CASE("Total number of nodes and grids is correct globally"){
    auto reader = Parfait::ParallelMeshReader({"../../grids/6cell.lb8.ugrid"}, {false});
    REQUIRE(14 == reader.totalNumberOfNodes());
    REQUIRE(1 == reader.numberOfGrids());
}
