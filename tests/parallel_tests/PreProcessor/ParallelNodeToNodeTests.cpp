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

    auto connectivity = std::make_shared<Parfait::MeshConnectivity>();
    auto metaData = std::make_shared<Parfait::MeshParallelMetaData>();
    if(MessagePasser::Rank() == 0){
        connectivity->prisms = {0,1,2,3,4,5};
        metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        metaData->globalNodeIds = {0,1,2,3,4,5};
        connectivity->triangles = {0,1,2};
        metaData->nodeComponentIds = {0,0,0,0,0,0};
        metaData->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(MessagePasser::Rank() == 1){
        connectivity->prisms = {3,4,5,0,1,2};
        metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        connectivity->triangles = {0,1,2};
        metaData->globalNodeIds = {3,4,5,0,1,2};
        metaData->nodeComponentIds = {0,0,0};
        metaData->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0};
    }

    Parfait::ParallelMesh mesh(connectivity, metaData);
    Parfait::ParallelNodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
    auto node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
    if(MessagePasser::Rank() == 0 or MessagePasser::Rank() == 1)
        REQUIRE(node_to_node.size() == 3);
}

