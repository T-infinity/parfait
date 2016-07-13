#include <catch.hpp>
#include <MessagePasser.h>
#include "ParallelMesh.h"
#include <memory>
#include <NodeCenteredRedistributor.h>

using namespace MessagePasser;

TEST_CASE("Node Centered Redistributor"){
    if(NumberOfProcesses() != 2)
        return;

    auto connectivity = std::make_shared<Parfait::MeshConnectivity>();
    auto metaData = std::make_shared<Parfait::MeshParallelMetaData>();
    std::vector<int> part(3, Rank());
    if(Rank() == 0){
        connectivity->prisms = {0,1,2,3,4,5};
        metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        metaData->globalNodeIds = {0,1,2,3,4,5};
        connectivity->triangles = {0,1,2};
        metaData->triangleTags = {1};
        metaData->nodeComponentIds = {0,0,0};
        metaData->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(Rank() == 1){
        connectivity->prisms = {3,4,5,0,1,2};
        metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        connectivity->triangles = {0,1,2};
        metaData->triangleTags = {1};
        metaData->globalNodeIds = {3,4,5,0,1,2};
        metaData->nodeComponentIds = {0,0,0};
        metaData->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0,};
    }

    auto mesh = std::make_shared<Parfait::ParallelMesh>(connectivity, metaData);
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