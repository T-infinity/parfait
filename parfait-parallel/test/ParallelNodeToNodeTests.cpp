#include <catch.hpp>
#include <memory>
#include <MessagePasser.h>
#include "ParallelMesh.h"
#include <ParallelNodeToNodeBuilder.h>

TEST_CASE("Redistribution Parallel Tests"){

    if(MessagePasser::NumberOfProcesses() < 2)
        return;

    Parfait::ParallelMeshBuilder meshBuilder;
    if(MessagePasser::Rank() == 0){
        meshBuilder.data->prisms = {0,1,2,3,4,5};
        meshBuilder.data->nodeOwnerRank = {0,0,0,1,1,1};
        meshBuilder.data->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->nodeComponentIds = {0,0,0,0,0,0};
        meshBuilder.data->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(MessagePasser::Rank() == 1){
        meshBuilder.data->prisms = {3,4,5,0,1,2};
        meshBuilder.data->nodeOwnerRank = {1,1,1,0,0,0};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.data->nodeComponentIds = {0,0,0};
        meshBuilder.data->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0};
    }

    Parfait::ParallelMesh mesh(meshBuilder);
    Parfait::ParallelNodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
    auto node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
    if(MessagePasser::Rank() == 0 or MessagePasser::Rank() == 1)
        REQUIRE(node_to_node.size() == 3);
}

