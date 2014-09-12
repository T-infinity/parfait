#include "CppUTest/CommandLineTestRunner.h"
#include "range_loop.h"
#include <stdio.h>
#include <algorithm>
#include <vector>

TEST_GROUP(RangeLoopTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(RangeLoopTests,Exists)
{
	using namespace std;
	auto myRange = range(0,2);
	auto begin = myRange.begin();
	auto end   = myRange.end();
	LONGS_EQUAL(0,*begin);
	LONGS_EQUAL(2,*end);

	++begin;
	LONGS_EQUAL(1,*begin);
	++begin;
	CHECK_EQUAL(*begin,*end);

	// print out 9-14
	for(int i:range(9,15))
		printf("range with lower bound %i\n",i);

	// print out 0-4
	for(int i:range(5))
		printf("auto zero lower bound %i\n",i);
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
