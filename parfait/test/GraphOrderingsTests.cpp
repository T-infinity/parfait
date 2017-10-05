#include <catch.hpp>
#include <MessagePasser/MessagePasser.h>
#include <parfait/GraphOrderings.h>

TEST_CASE("contiguous row start"){
    auto mp = std::make_shared<MessagePasser>();
    if(mp->NumberOfProcesses() != 2) return;

    int num_owned_nodes = 10;
    auto start = Parfait::GraphOrderings::getRankRowStart(mp, num_owned_nodes);
    if(mp->Rank() == 0) REQUIRE(start == 0);
    if(mp->Rank() == 1) REQUIRE(start == 10);
}

TEST_CASE("Check have need"){
    auto local_to_global = std::vector<long> {0, 5, 4, 2, 7};
    auto do_own_local = std::vector<bool> {true, true, false, true, true};
    std::vector<long> have, need;
    std::tie(have, need) = Parfait::GraphOrderings::buildHaveNeed(local_to_global, do_own_local);
    REQUIRE(have.size() == 4);
    REQUIRE(need.size() == 1);
}

TEST_CASE("build local to compact owned"){
    auto local_to_global = std::vector<long> {0, 5, 4, 2, 7};
    auto do_own_local_node = std::vector<bool> {true, true, false, true, true};
    auto local_to_compact = Parfait::GraphOrderings::buildLocalToCompactOwned(do_own_local_node);
    REQUIRE(local_to_compact.size() == local_to_global.size());
    REQUIRE(local_to_compact[0] == 0);
    REQUIRE(local_to_compact[1] == 1);
    REQUIRE(local_to_compact[2] ==-1);
    REQUIRE(local_to_compact[3] == 2);
    REQUIRE(local_to_compact[4] == 3);
}

TEST_CASE("Assign owned contiguous global orderings"){
    auto local_to_global = std::vector<long> {0, 5, 4, 2, 7};
    auto do_own_local_node = std::vector<bool> {true, true, false, true, true};
    auto local_to_compact = Parfait::GraphOrderings::buildLocalToCompactOwned(do_own_local_node);
    long contiguous_row_start = 100;
    auto local_to_contiguous_global =
        Parfait::GraphOrderings::assignOwnedContiguousGlobal(do_own_local_node, contiguous_row_start, local_to_compact);
    REQUIRE(local_to_contiguous_global.size() == 5);
    REQUIRE(local_to_contiguous_global[0] == 100);
    REQUIRE(local_to_contiguous_global[1] == 101);
    REQUIRE(local_to_contiguous_global[2] == -1);
    REQUIRE(local_to_contiguous_global[3] == 102);
    REQUIRE(local_to_contiguous_global[4] == 103);
}

TEST_CASE("Make global to global from local to global and local to global"){
    auto local_to_global_1 = std::vector<long>{5,4,3,2,1,0};
    auto local_to_global_2 = std::vector<long>{1,0,3,2,5,4};
    auto global_1_to_global_2 = Parfait::GraphOrderings::buildGlobalToGlobalMap(local_to_global_1, local_to_global_2);
    REQUIRE(global_1_to_global_2.size() == 6);
    REQUIRE(global_1_to_global_2[5] == 1);
    REQUIRE(global_1_to_global_2[4] == 0);
    REQUIRE(global_1_to_global_2[3] == 3);
    REQUIRE(global_1_to_global_2[2] == 2);
    REQUIRE(global_1_to_global_2[1] == 5);
    REQUIRE(global_1_to_global_2[0] == 4);
}

TEST_CASE("distributed to contiguous, serial"){
    auto mp = std::make_shared<MessagePasser>();
    if(mp->NumberOfProcesses() != 1) return;

    std::vector<long> local_to_global = {13,1,2,3,4,5,6,7,8,9,10,11,12,0};
    std::vector<bool> do_own(14, true);
    std::vector<std::vector<long>> columns = {
        {6,7,8,9,10,11,12,13}, // 13
        {0,1,2,6,8},           // 1
        {1,2,3,6,9},           // 2
        {2,3,4,6,10},          // 3
        {3,4,5,6,11},          // 4
        {0,4,5,6,12},          // 5
        {0,1,2,3,4,5,6,13},    // 6
        {0,7,8,12,13},         // 7
        {1,7,8,9,13},          // 8
        {2,8,9,10,13},         // 9
        {3,9,10,11,13},        // 10
        {4,10,11,12,13},       // 11
        {5,7,11,12,13},        // 12
        {0,1,5,6,7}            // 0
    };

    auto distributed_to_contiguous = Parfait::GraphOrderings::buildDistributedToContiguous(MPI_COMM_WORLD, local_to_global, do_own);
    REQUIRE(distributed_to_contiguous.size() == 14);
    for(int i = 0; i < distributed_to_contiguous.size(); i++){
        REQUIRE(local_to_global[i] == distributed_to_contiguous[i]);
    }
}

TEST_CASE("distributed to contiguous, parallel, no non-owned residents in local_to_global array") {
    auto mp = std::make_shared<MessagePasser>();
    if(mp->NumberOfProcesses() != 2) return;

    std::vector<long> local_to_global;
    std::vector<std::vector<long>>columns;
    std::vector<bool> own_local_node(4, true);
    if(mp->Rank() == 0) {
        local_to_global = std::vector<long> {0, 5, 2, 7};
    }
    else if(mp->Rank() == 1){
        local_to_global = std::vector<long> {4, 1, 6, 3};
    }

    auto distributed_to_contiguous =
        Parfait::GraphOrderings::buildDistributedToContiguous(mp->getCommunicator(), local_to_global, own_local_node);
    if(mp->Rank() == 0){
        REQUIRE(distributed_to_contiguous[0] == 0);
        REQUIRE(distributed_to_contiguous[5] == 1);
        REQUIRE(distributed_to_contiguous[2] == 2);
        REQUIRE(distributed_to_contiguous[7] == 3);
    } else if(mp->Rank() == 1){
        REQUIRE(distributed_to_contiguous[4] == 4);
        REQUIRE(distributed_to_contiguous[1] == 5);
        REQUIRE(distributed_to_contiguous[6] == 6);
        REQUIRE(distributed_to_contiguous[3] == 7);
    }
}

TEST_CASE("Build global to local"){
    auto local_to_global = std::vector<long> {0, 5, 4, 2, 7};
    auto global_to_local = Parfait::GraphOrderings::buildGlobalToLocal(local_to_global);
    REQUIRE(global_to_local.size() == 5);
    REQUIRE(0 == global_to_local[0]);
    REQUIRE(1 == global_to_local[5]);
    REQUIRE(2 == global_to_local[4]);
    REQUIRE(3 == global_to_local[2]);
    REQUIRE(4 == global_to_local[7]);
}

TEST_CASE("distributed to contiguous in parallel, with non-owned residents in the local_to_global array"){
    auto mp = std::make_shared<MessagePasser>();
    if(mp->NumberOfProcesses() != 2) return;

    std::vector<long> local_to_global;
    std::vector<std::vector<long>>columns;
    std::vector<bool> own_local_node;
    if(mp->Rank() == 0) {
        local_to_global = std::vector<long> {0, 5, 4, 2, 7};
        own_local_node = std::vector<bool> {true, true, false, true, true};
    }
    else if(mp->Rank() == 1){
        local_to_global = std::vector<long> {4, 5, 1, 6, 3};
        own_local_node = std::vector<bool>{true, false, true, true, true};
    }

    auto distributed_to_contiguous =
        Parfait::GraphOrderings::buildDistributedToContiguous(mp->getCommunicator(), local_to_global, own_local_node);
    if(mp->Rank() == 0){
        REQUIRE(distributed_to_contiguous[0] == 0);
        REQUIRE(distributed_to_contiguous[5] == 1);
        REQUIRE(distributed_to_contiguous[2] == 2);
        REQUIRE(distributed_to_contiguous[7] == 3);
        REQUIRE(distributed_to_contiguous[4] == 4);
    } else if(mp->Rank() == 1){
        REQUIRE(distributed_to_contiguous[4] == 4);
        REQUIRE(distributed_to_contiguous[1] == 5);
        REQUIRE(distributed_to_contiguous[6] == 6);
        REQUIRE(distributed_to_contiguous[3] == 7);
        REQUIRE(distributed_to_contiguous[5] == 1);
    }
}

