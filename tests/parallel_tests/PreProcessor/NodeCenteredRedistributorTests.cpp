#include <catch.hpp>
#include <MessagePasser.h>
#include "ParallelMesh.h"
#include <memory>
#include <NodeCenteredRedistributor.h>

using namespace MessagePasser;

TEST_CASE("Node Centered Redistributor"){
    if(NumberOfProcesses() != 2)
        return;

    auto mesh = std::make_shared<Parfait::ParallelMesh>();
    std::vector<int> part(3, Rank());
    if(Rank() == 0){
        mesh->connectivity->prisms = {0,1,2,3,4,5};
        mesh->metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        mesh->metaData->globalNodeIds = {0,1,2,3,4,5};
        mesh->connectivity->triangles = {0,1,2};
        mesh->metaData->triangleTags = {1};
        mesh->metaData->nodeComponentIds = {0,0,0};
        mesh->metaData->xyz = {0,0,0, 1,0,0, 1,1,0};
    }

    if(Rank() == 1){
        mesh->connectivity->prisms = {3,4,5,0,1,2};
        mesh->metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        mesh->connectivity->triangles = {0,1,2};
        mesh->metaData->triangleTags = {1};
        mesh->metaData->globalNodeIds = {3,4,5,0,1,2};
        mesh->metaData->nodeComponentIds = {0,0,0};
        mesh->metaData->xyz = {0,0,1, 1,0,1, 1,1,1};
    }

    Parfait::ParallelMeshReDistributor reDistributor(mesh, part);
    mesh = reDistributor.redistribute();

    int number_of_owned_nodes = 0;
    for(int d : mesh->metaData->nodeOwnershipDegree)
        if(d == 0)
            number_of_owned_nodes++;

    auto total = MessagePasser::ParallelSum(number_of_owned_nodes);
    REQUIRE(6 == total);

    REQUIRE(mesh->metaData->xyz.size() == 6*3);
}

