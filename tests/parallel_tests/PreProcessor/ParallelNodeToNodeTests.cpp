#include <catch.hpp>
#include <memory>
#include <MessagePasser.h>
#include "ParallelMesh.h"
#include <PartitionableMesh.h>
#include <ParallelPartitionableMesh.h>
#include <ParallelNodeToNodeBuilder.h>

TEST_CASE("Redistribution Parallel Tests"){

    if(MessagePasser::NumberOfProcesses() < 2)
        return;

    auto mesh = std::make_shared<Parfait::ParallelMesh>();
    if(MessagePasser::Rank() == 0){
        mesh->connectivity->prisms = {0,1,2,3,4,5};
        mesh->metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        mesh->metaData->globalNodeIds = {0,1,2};
        mesh->connectivity->triangles = {0,1,2};
        mesh->metaData->nodeComponentIds = {0,0,0};
        mesh->metaData->xyz = {0,0,0, 1,0,0, 1,1,0};
    }

    if(MessagePasser::Rank() == 1){
        mesh->connectivity->prisms = {3,4,5,0,1,2};
        mesh->metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        mesh->connectivity->triangles = {0,1,2};
        mesh->metaData->globalNodeIds = {3,4,5,0,1,2};
        mesh->metaData->nodeComponentIds = {0,0,0};
        mesh->metaData->xyz = {0,0,1, 1,0,1, 1,1,1};
    }
    Parfait::ParallelPartitionableMesh partitionableMesh(mesh);
    Parfait::ParallelNodeToNodeBuilder<Parfait::ParallelPartitionableMesh> nodeBuilder(partitionableMesh);
    auto node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
    if(MessagePasser::Rank() == 0 or MessagePasser::Rank() == 1)
        REQUIRE(node_to_node.size() == 3);
}

