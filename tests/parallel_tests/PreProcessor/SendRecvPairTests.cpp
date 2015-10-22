#include <catch.hpp>
#include <MessagePasser.h>
#include <ParallelMesh.h>
#include <NodeCenteredRedistributor.h>
#include <SendReceivePairCreator.h>

TEST_CASE("Send Recv Pairs Exist"){
    if(MessagePasser::NumberOfProcesses() != 2) return;

    auto mesh = std::make_shared<Parfait::ParallelMesh>();
    if(MessagePasser::Rank() == 0){
        mesh->connectivity->prisms = {0,1,2,3,4,5};
        mesh->metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        mesh->metaData->globalNodeIds = {0,1,2,3,4,5};
        mesh->connectivity->triangles = {0,1,2};
        mesh->metaData->triangleTags = {1};
        mesh->metaData->nodeComponentIds = {0,0,0};
        mesh->metaData->xyz = {0,0,0, 1,0,0, 1,1,0};
    }
    else if (MessagePasser::Rank() == 1){
        mesh->connectivity->prisms = {3,4,5,0,1,2};
        mesh->metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        mesh->connectivity->triangles = {0,1,2};
        mesh->metaData->triangleTags = {1};
        mesh->metaData->globalNodeIds = {3,4,5,0,1,2};
        mesh->metaData->nodeComponentIds = {0,0,0};
        mesh->metaData->xyz = {0,0,1, 1,0,1, 1,1,1};
    }
    std::vector<int> part(3, MessagePasser::Rank());
    Parfait::NodeBasedRedistributor reDistributor(mesh, part);
    mesh = reDistributor.redistribute();

    auto pair = Parfait::SendReceivePairCreator::createNodeBasedPair(mesh);
    REQUIRE(pair.send.size() == 3);
    int other_rank = MessagePasser::Rank() == 0 ? 1 : 0;
    REQUIRE(pair.send[0].size() == 1);
}