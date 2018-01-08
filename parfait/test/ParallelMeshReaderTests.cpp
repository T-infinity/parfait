#include <string>
#include <catch.hpp>
#include <parfait/ParallelMeshReader.h>


TEST_CASE("Read in the grid and check it if run on one proc") {
    MessagePasser mp;
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid(
            mp, {"../../grids/6cell.lb8.ugrid"}, {false});
    if(mp.NumberOfProcesses() == 1){
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
    MessagePasser mp;
    auto reader = Parfait::ParallelMeshReader(mp, {"../../grids/6cell.lb8.ugrid"}, {false});
    REQUIRE(14 == reader.totalNumberOfNodes());
    REQUIRE(1 == reader.numberOfGrids());
}
