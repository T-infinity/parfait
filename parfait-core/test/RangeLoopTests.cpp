#include <catch.hpp>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "RangeLoop.h"

TEST_CASE("RangeLoopTests,Exists") {
    using namespace std;
    auto myRange = range(0, 2);
    auto begin = myRange.begin();
    auto end = myRange.end();
    REQUIRE(0 == *begin);
    REQUIRE(2 == *end);

    ++begin;
    REQUIRE(1 == *begin);
    ++begin;
    REQUIRE(*begin == *end);
}
TEST_CASE("RangeLoopTests, InsideARange") {
    std::vector<int> vec = {9, 10, 11, 12, 13, 14};
    int index = 0;
    for (int i:range(9, 15)) REQUIRE(vec[index++] == i);
}

TEST_CASE("RangeLoopTests, SingleNumber")
{
    std::vector<int> vec = {0, 1, 2, 3, 4};
    int index = 0;
    for (int i:range(5)) REQUIRE(vec[index++] == i);
}

TEST_CASE("RangeLoopTests,AutoVectorIndex"){
	using std::vector;
	vector<int> vec(3,0);
    int index = 0;
	for(int i:range(vec)) REQUIRE(index++ == i);
}