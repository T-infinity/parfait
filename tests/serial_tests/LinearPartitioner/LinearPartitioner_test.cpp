#include "LinearPartitioner.h"
#include "CppUTest/CommandLineTestRunner.h"

using namespace Parfait;
TEST_GROUP(LinearPartitionerTests){};

TEST(LinearPartitionerTests, GetWorkerOfWorkItem){
    auto id = LinearPartitioner::getWorkerOfWorkItem(0, 10, 1);
    LONGS_EQUAL(id, 0);

    id = LinearPartitioner::getWorkerOfWorkItem(0, 10, 2);
    LONGS_EQUAL(id, 0);
    id = LinearPartitioner::getWorkerOfWorkItem(5, 10, 2);
    LONGS_EQUAL(id, 1);
    id = LinearPartitioner::getWorkerOfWorkItem(6, 10, 2);
    LONGS_EQUAL(id, 1);

    id = LinearPartitioner::getWorkerOfWorkItem(3, 10, 3);
    LONGS_EQUAL(id, 0);
    id = LinearPartitioner::getWorkerOfWorkItem(4, 10, 3);
    LONGS_EQUAL(id, 1);
    id = LinearPartitioner::getWorkerOfWorkItem(5, 10, 3);
    LONGS_EQUAL(id, 1);
    id = LinearPartitioner::getWorkerOfWorkItem(6, 10, 3);
    LONGS_EQUAL(id, 1);
}

TEST(LinearPartitionerTests, getRangeOfWorker){
    auto range = LinearPartitioner::getRangeForWorker(0, 1, 1);

    LONGS_EQUAL(range.start, 0);
    LONGS_EQUAL(range.end, 1);

    range = LinearPartitioner::getRangeForWorker(0, 100, 10);
    LONGS_EQUAL(range.start, 0);
    LONGS_EQUAL(range.end, 10);

    range = LinearPartitioner::getRangeForWorker(0, 101, 10);
    LONGS_EQUAL(range.start, 0);
    LONGS_EQUAL(range.end, 11);

    range = LinearPartitioner::getRangeForWorker(1, 101, 10);
    LONGS_EQUAL(range.start, 11);
    LONGS_EQUAL(range.end, 21);

    range = LinearPartitioner::getRangeForWorker(9, 101, 10);
    LONGS_EQUAL(range.start, 91);
    LONGS_EQUAL(range.end, 101);

    range = LinearPartitioner::getRangeForWorker(9, 624, 10);
    CHECK(range.end - range.start > 55);

    CHECK_THROWS(std::logic_error, LinearPartitioner::getRangeForWorker(10, 101, 10));
    CHECK_THROWS(std::logic_error, LinearPartitioner::getRangeForWorker(-1, 101, 10));
}

