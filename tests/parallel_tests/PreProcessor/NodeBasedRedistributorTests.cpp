#include <catch.hpp>
#include <MessagePasser.h>
#include "ParallelMesh.h"
#include <memory>
#include <NodeCenteredRedistributor.h>

using namespace MessagePasser;

TEST_CASE("Node Centered Redistributor"){
    if(NumberOfProcesses() != 2)
        return;

    Parfait::ParallelMeshBuilder meshBuilder;
    std::vector<int> part(3, Rank());
    if(Rank() == 0){
        meshBuilder.connectivity->prisms = {0,1,2,3,4,5};
        meshBuilder.metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        meshBuilder.metaData->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.connectivity->triangles = {0,1,2};
        meshBuilder.metaData->triangleTags = {1};
        meshBuilder.metaData->nodeComponentIds = {0,0,0};
        meshBuilder.metaData->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(Rank() == 1){
        meshBuilder.connectivity->prisms = {3,4,5,0,1,2};
        meshBuilder.metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        meshBuilder.connectivity->triangles = {0,1,2};
        meshBuilder.metaData->triangleTags = {1};
        meshBuilder.metaData->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.metaData->nodeComponentIds = {0,0,0};
        meshBuilder.metaData->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0,};
    }

    auto mesh = std::make_shared<Parfait::ParallelMesh>(meshBuilder);
    Parfait::NodeBasedRedistributor reDistributor(mesh, part);
    mesh = reDistributor.redistribute();

    int number_of_owned_nodes = 0;
    for(int d = 0; d < mesh->numberOfNodes(); d++){
        if(not mesh->isGhostNode(d))
            number_of_owned_nodes++;
    }

    auto total = MessagePasser::ParallelSum(number_of_owned_nodes);
    REQUIRE(6 == total);

    REQUIRE(mesh->numberOfNodes() == 6);
}