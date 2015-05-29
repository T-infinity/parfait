#include "range_loop.h"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(RangeLoopTests)
{
};

TEST(RangeLoopTests,Exists) {
    using namespace std;
    auto myRange = range(0, 2);
    auto begin = myRange.begin();
    auto end = myRange.end();
    LONGS_EQUAL(0, *begin);
    LONGS_EQUAL(2, *end);

    ++begin;
    LONGS_EQUAL(1, *begin);
    ++begin;
    CHECK_EQUAL(*begin, *end);
}
TEST(RangeLoopTests, InsideARange)
{
    std::vector<int> vec = {9, 10, 11, 12, 13, 14};
    int index = 0;
    for (int i:range(9, 15)) LONGS_EQUAL(vec[index++], i);
}

TEST(RangeLoopTests, SingleNumber)
{
    std::vector<int> vec = {0, 1, 2, 3, 4};
    int index = 0;
    for (int i:range(5)) LONGS_EQUAL(vec[index++], i);
}

TEST(RangeLoopTests,AutoVectorIndex){
	using std::vector;
	vector<int> vec(3,0);
    int index = 0;
	for(int i:range(vec)) LONGS_EQUAL(index++, i);
}