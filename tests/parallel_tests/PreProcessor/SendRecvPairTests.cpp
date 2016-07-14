#include <catch.hpp>
#include <MessagePasser.h>
#include <ParallelMesh.h>
#include <NodeCenteredRedistributor.h>
#include <SendReceivePairCreator.h>

std::shared_ptr<Parfait::ParallelMesh> getTestMesh(){

    Parfait::ParallelMeshBuilder meshBuilder;
    std::vector<int> part(3, MessagePasser::Rank());
    if(MessagePasser::Rank() == 0){
        meshBuilder.connectivity->prisms = {0,1,2,3,4,5};
        meshBuilder.metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        meshBuilder.metaData->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.connectivity->triangles = {0,1,2};
        meshBuilder.metaData->triangleTags = {1};
        meshBuilder.metaData->nodeComponentIds = {0,0,0};
        meshBuilder.metaData->xyz = {0,0,0, 1,0,0, 1,1,0};
    }

    if(MessagePasser::Rank() == 1){
        meshBuilder.connectivity->prisms = {3,4,5,0,1,2};
        meshBuilder.metaData->nodeOwnershipDegree = {0,0,0,1,1,1};
        meshBuilder.connectivity->triangles = {0,1,2};
        meshBuilder.metaData->triangleTags = {1};
        meshBuilder.metaData->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.metaData->nodeComponentIds = {0,0,0};
        meshBuilder.metaData->xyz = {0,0,1, 1,0,1, 1,1,1};
    }
    auto mesh = std::make_shared<Parfait::ParallelMesh>(meshBuilder);
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
    REQUIRE(sendReceivePair.recv.size() == 6);

    if (MessagePasser::Rank() == 0) {
        REQUIRE(sendReceivePair.send[0].size() == 1);
        REQUIRE(sendReceivePair.send[1].size() == 1);
        REQUIRE(sendReceivePair.send[2].size() == 1);
        REQUIRE(sendReceivePair.send[3].size() == 0);
        REQUIRE(sendReceivePair.send[4].size() == 0);
        REQUIRE(sendReceivePair.send[5].size() == 0);
        REQUIRE((sendReceivePair.send[0][0] == other[MessagePasser::Rank()]));
        REQUIRE((sendReceivePair.send[1][0] == other[MessagePasser::Rank()]));
        REQUIRE((sendReceivePair.send[2][0] == other[MessagePasser::Rank()]));
        REQUIRE(sendReceivePair.recv[0] == -1);
        REQUIRE(sendReceivePair.recv[1] == -1);
        REQUIRE(sendReceivePair.recv[2] == -1);
        REQUIRE(sendReceivePair.recv[3] == 1);
        REQUIRE(sendReceivePair.recv[4] == 1);
        REQUIRE(sendReceivePair.recv[5] == 1);
    }
    if (MessagePasser::Rank() == 1) {
        REQUIRE(sendReceivePair.send[0].size() == 0);
        REQUIRE(sendReceivePair.send[1].size() == 0);
        REQUIRE(sendReceivePair.send[2].size() == 0);
        REQUIRE(sendReceivePair.send[3].size() == 1);
        REQUIRE(sendReceivePair.send[4].size() == 1);
        REQUIRE(sendReceivePair.send[5].size() == 1);
        REQUIRE((sendReceivePair.send[3][0] == other[MessagePasser::Rank()]));
        REQUIRE((sendReceivePair.send[4][0] == other[MessagePasser::Rank()]));
        REQUIRE((sendReceivePair.send[5][0] == other[MessagePasser::Rank()]));
        REQUIRE(sendReceivePair.recv[0] == 0);
        REQUIRE(sendReceivePair.recv[1] == 0);
        REQUIRE(sendReceivePair.recv[2] == 0);
        REQUIRE(sendReceivePair.recv[3] == -1);
        REQUIRE(sendReceivePair.recv[4] == -1);
        REQUIRE(sendReceivePair.recv[5] == -1);
    }
}