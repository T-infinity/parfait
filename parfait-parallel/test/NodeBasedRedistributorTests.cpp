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
        meshBuilder.data->prisms = {0,1,2,3,4,5};
        meshBuilder.data->nodeOwnerRank = {0,0,0,1,1,1};
        meshBuilder.data->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->triangleTags = {1};
        meshBuilder.data->nodeComponentIds = {0,0,0};
        meshBuilder.data->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(Rank() == 1){
        meshBuilder.data->prisms = {3,4,5,0,1,2};
        meshBuilder.data->nodeOwnerRank = {1,1,1,0,0,0};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->triangleTags = {1};
        meshBuilder.data->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.data->nodeComponentIds = {0,0,0};
        meshBuilder.data->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0,};
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
