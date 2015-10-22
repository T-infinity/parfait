#include <catch.hpp>
#include <MessagePasser.h>
#include <ParallelMesh.h>
#include <NodeCenteredRedistributor.h>
#include <SendReceivePairCreator.h>

std::shared_ptr<Parfait::ParallelMesh> getTestMesh(){
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
    return mesh;
}

TEST_CASE("Send Recv Creator determine what nodes I have and which I need") {
    if(MessagePasser::NumberOfProcesses() != 2) return;
    std::shared_ptr<Parfait::ParallelMesh> mesh = getTestMesh();
    std::vector<int> part(3, MessagePasser::Rank());
    Parfait::NodeBasedRedistributor reDistributor(mesh, part);
    mesh = reDistributor.redistribute();

    Parfait::SendReceivePairCreator::NodePairCreator creator(mesh);
    auto needed = creator.findNeededNodes();
    if(MessagePasser::Rank() == 0){
        REQUIRE((needed == std::vector<long>{3,4,5}));
    }
    if(MessagePasser::Rank() == 1){
        REQUIRE((needed == std::vector<long>{0,1,2}));
    }

    auto owned = creator.getOwnedNodeGlobalIds();
    if(MessagePasser::Rank() == 0){
        REQUIRE((owned == std::vector<long>{0,1,2}));
    }
    if(MessagePasser::Rank() == 1){
        REQUIRE((owned == std::vector<long>{3,4,5}));
    }
}

TEST_CASE("Test that we can find correctly determine if we need global nodes"){
    if(MessagePasser::NumberOfProcesses() != 2) return;
    std::shared_ptr<Parfait::ParallelMesh> mesh = getTestMesh();
    std::vector<int> part(3, MessagePasser::Rank());
    Parfait::NodeBasedRedistributor reDistributor(mesh, part);
    mesh = reDistributor.redistribute();

    Parfait::SendReceivePairCreator::NodePairCreator creator(mesh);

    std::vector<long> remote_nodes;
    if(MessagePasser::Rank() == 0)
        remote_nodes = std::vector<long>{3,4,5};
    if(MessagePasser::Rank() == 1)
        remote_nodes = std::vector<long>{0,1,2};

    auto needed_nodes = creator.findNeededNodes();
    auto do_need = creator.doNeedRemoteNodes(needed_nodes, remote_nodes);
    if(MessagePasser::Rank() == 0)
        REQUIRE((do_need == std::vector<long>{3,4,5}));
    if(MessagePasser::Rank() == 1)
        REQUIRE((do_need == std::vector<long>{0,1,2}));
}

TEST_CASE("Create Correct Send Receive Pairs") {
    if (MessagePasser::NumberOfProcesses() != 2) return;
    std::shared_ptr<Parfait::ParallelMesh> mesh = getTestMesh();
    std::vector<int> part(3, MessagePasser::Rank());
    Parfait::NodeBasedRedistributor reDistributor(mesh, part);
    mesh = reDistributor.redistribute();

    Parfait::SendReceivePairCreator::NodePairCreator creator(mesh);

    auto sendReceivePair = creator.create();

    std::array<int, 2> other = {1, 0};

    REQUIRE(sendReceivePair.send.size() == 6);
    if(MessagePasser::Rank() == 0) {
        REQUIRE(sendReceivePair.send[0].size() == 1);
        REQUIRE(sendReceivePair.send[1].size() == 1);
        REQUIRE(sendReceivePair.send[2].size() == 1);
        REQUIRE(sendReceivePair.send[3].size() == 0);
        REQUIRE(sendReceivePair.send[4].size() == 0);
        REQUIRE(sendReceivePair.send[5].size() == 0);
        REQUIRE((sendReceivePair.send[0][0] == other[MessagePasser::Rank()]));
        REQUIRE((sendReceivePair.send[1][0] == other[MessagePasser::Rank()]));
        REQUIRE((sendReceivePair.send[2][0] == other[MessagePasser::Rank()]));
    }


}