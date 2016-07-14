#include <catch.hpp>
#include <memory>
#include <MessagePasser.h>
#include "ParallelMesh.h"
#include <ParallelNodeToNodeBuilder.h>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"

TEST_CASE("Redistribution Parallel Tests"){

    if(MessagePasser::NumberOfProcesses() < 2)
        return;

    Parfait::ParallelMeshBuilder meshBuilder;
    if(MessagePasser::Rank() == 0){
        meshBuilder.connectivity->prisms = {0,1,2,3,4,5};
        meshBuilder.metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        meshBuilder.metaData->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.connectivity->triangles = {0,1,2};
        meshBuilder.metaData->nodeComponentIds = {0,0,0,0,0,0};
        meshBuilder.metaData->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(MessagePasser::Rank() == 1){
        meshBuilder.connectivity->prisms = {3,4,5,0,1,2};
        meshBuilder.metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        meshBuilder.connectivity->triangles = {0,1,2};
        meshBuilder.metaData->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.metaData->nodeComponentIds = {0,0,0};
        meshBuilder.metaData->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0};
    }

    Parfait::ParallelMesh mesh(meshBuilder);
    Parfait::ParallelNodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
    auto node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
    if(MessagePasser::Rank() == 0 or MessagePasser::Rank() == 1)
        REQUIRE(node_to_node.size() == 3);
}

