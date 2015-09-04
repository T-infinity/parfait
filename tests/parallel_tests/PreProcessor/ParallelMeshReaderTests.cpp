#include <string>
#include "PreProcessor.h"
#include <ParallelMeshReader.h>
#include <catch.hpp>

using std::string;

TEST_CASE("Read in the grid and check it if run on one proc") {
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid({"../../grids/6cell.lb8.ugrid"}, {false});
    if(MessagePasser::NumberOfProcesses() == 1){
        REQUIRE(12 == (mesh.triangles.size() / 3));
        REQUIRE(12 == mesh.triangleTags.size());
        REQUIRE(6 == (mesh.quads.size() / 4));
        REQUIRE(0 == (mesh.tets.size() / 4));
        REQUIRE(0 == (mesh.pyramids.size() / 5));
        REQUIRE(6 == (mesh.prisms.size() / 6));
        REQUIRE(0 == (mesh.hexs.size() / 6));
        for(int i = 0; i < 14; i++){
            REQUIRE(i == mesh.getGlobalNodeId(i));
        }
        for(int i = 0; i < 14; i++){
            REQUIRE(0 == mesh.ownershipDegreeOfLocalNode(i));
        }
        REQUIRE(14 == mesh.numberOfNodesOfDegreeOrUnder(0));
    }
}

TEST_CASE("Total number of nodes and grids is correct globally"){
    auto reader = Parfait::ParallelMeshReader({"../../grids/6cell.lb8.ugrid"}, {false});
    REQUIRE(14 == reader.totalNumberOfNodes());
    REQUIRE(1 == reader.numberOfGrids());
}
