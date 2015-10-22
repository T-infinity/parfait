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

TEST_CASE("Send Recv Pairs") {
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