#include <catch.hpp>
#include "LinearPartitioner.h"

using namespace Parfait;

TEST_CASE("LinearPartitionerTests, GetWorkerOfWorkItem"){
    auto id = LinearPartitioner::getWorkerOfWorkItem(0, 10, 1);
    REQUIRE(id == 0);

    id = LinearPartitioner::getWorkerOfWorkItem(0, 10, 2);
    REQUIRE(id == 0);
    id = LinearPartitioner::getWorkerOfWorkItem(5, 10, 2);
    REQUIRE(id == 1);
    id = LinearPartitioner::getWorkerOfWorkItem(6, 10, 2);
    REQUIRE(id == 1);

    id = LinearPartitioner::getWorkerOfWorkItem(3, 10, 3);
    REQUIRE(id == 0);
    id = LinearPartitioner::getWorkerOfWorkItem(4, 10, 3);
    REQUIRE(id == 1);
    id = LinearPartitioner::getWorkerOfWorkItem(5, 10, 3);
    REQUIRE(id == 1);
    id = LinearPartitioner::getWorkerOfWorkItem(6, 10, 3);
    REQUIRE(id == 1);
}

TEST_CASE("LinearPartitionerTests, getRangeOfWorker"){
    auto range = LinearPartitioner::getRangeForWorker(0, 1, 1);

    REQUIRE(range.start == 0);
    REQUIRE(range.end == 1);

    range = LinearPartitioner::getRangeForWorker(0, 100, 10);
    REQUIRE(range.start == 0);
    REQUIRE(range.end == 10);

    range = LinearPartitioner::getRangeForWorker(0, 101, 10);
    REQUIRE(range.start == 0);
    REQUIRE(range.end == 11);

    range = LinearPartitioner::getRangeForWorker(1, 101, 10);
    REQUIRE(range.start == 11);
    REQUIRE(range.end == 21);

    range = LinearPartitioner::getRangeForWorker(9, 101, 10);
    REQUIRE(range.start == 91);
    REQUIRE(range.end == 101);

    range = LinearPartitioner::getRangeForWorker(9, 624, 10);
    REQUIRE((range.end - range.start > 55));

    REQUIRE_THROWS(LinearPartitioner::getRangeForWorker(10, 101, 10));
    REQUIRE_THROWS(LinearPartitioner::getRangeForWorker(-1, 101, 10));
}

TEST_CASE("LinearPartitionerTests, getLocalId"){
    auto ownerIdPair = LinearPartitioner::getOwnerLocalIdPairOfGlobalItem(0, 1, 1);
    REQUIRE(0 == ownerIdPair.owner);
    REQUIRE(0 == ownerIdPair.id);

    ownerIdPair = LinearPartitioner::getOwnerLocalIdPairOfGlobalItem(1, 10, 10);
    REQUIRE(1 == ownerIdPair.owner);
    REQUIRE(0 == ownerIdPair.id);

    auto localId = LinearPartitioner::getLocalIdOfItemOnWorker(0, 0, 10, 10);
    REQUIRE(0 ==localId);
    localId = LinearPartitioner::getLocalIdOfItemOnWorker(0, 1, 10, 10);
    REQUIRE(-1 == localId);
    localId = LinearPartitioner::getLocalIdOfItemOnWorker(1, 0, 10, 10);
    REQUIRE(1 == localId);
}

template <typename T>
bool rangeOwns(const LinearPartitioner::Range<T> &range, T id) {
    return range.owns(id);
}

TEST_CASE("Range owned"){
    REQUIRE(rangeOwns({0, 100}, 0));
    REQUIRE(rangeOwns({0, 100}, 5));
    REQUIRE(not rangeOwns({0,100}, 100));
    REQUIRE(not rangeOwns({0,100}, 1000));
}


TEST_CASE("Range owns large data types"){
    typedef unsigned long int MortonId;
    REQUIRE(not rangeOwns(LinearPartitioner::Range<MortonId> {0, 1152921504606846976}, (MortonId)1152921504606846977));
}