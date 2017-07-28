#include <catch.hpp>
#include <memory>
#include <MessagePasser/MessagePasser.h>
#include <parfait/ParallelMesh.h>
#include <parfait/ParallelNodeToNodeBuilder.h>

TEST_CASE("Redistribution Parallel Tests"){
    auto mp = std::make_shared<MessagePasser>();


    if(mp->NumberOfProcesses() < 2)
        return;

    Parfait::ParallelMeshBuilder meshBuilder(mp);
    if(mp->Rank() == 0){
        meshBuilder.data->prisms = {0,1,2,3,4,5};
        meshBuilder.data->nodeOwnerRank = {0,0,0,1,1,1};
        meshBuilder.data->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->nodeComponentIds = {0,0,0,0,0,0};
        meshBuilder.data->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(mp->Rank() == 1){
        meshBuilder.data->prisms = {3,4,5,0,1,2};
        meshBuilder.data->nodeOwnerRank = {1,1,1,0,0,0};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.data->nodeComponentIds = {0,0,0};
        meshBuilder.data->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0};
    }

    Parfait::ParallelMesh mesh(mp, meshBuilder);
    Parfait::ParallelNodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
    auto node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
    if(mp->Rank() == 0 or mp->Rank() == 1)
        REQUIRE(node_to_node.size() == 3);
}

