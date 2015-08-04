#include "LinearPartitioner.h"
#include "CppUTest/CommandLineTestRunner.h"

using namespace Parfait;
TEST_GROUP(LinearPartitionerTests){};

TEST(LinearPartitionerTests, Exists){
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

    CHECK_THROWS(std::logic_error, LinearPartitioner::getRangeForWorker(10, 101, 10));
    CHECK_THROWS(std::logic_error, LinearPartitioner::getRangeForWorker(-1, 101, 10));
}

