#include "CppUTest/CommandLineTestRunner.h"
#include "range_loop.h"
#include <stdio.h>
#include <algorithm>
#include <vector>

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


TEST(RangeLoopTests,AutoVectorIndex)
{
	using std::vector;
	vector<int> vec(3,0);
	// you can do a range loop over the indices of
	// any container with a random access iterator!
	for(int i:range(vec))
		printf("range in vector: %i\n",i);
}

TEST(RangeLoopTests,AutoVectorWithStride)
{
	using std::vector;
	vector<int> triangles(12,0);
	for(int i:range(triangles,3))
	{
		printf("range loop over vector with a stride %i\n",i);
		// do something with
		// triangles[3*i], triangles[3*i+1], etc
	}
}
