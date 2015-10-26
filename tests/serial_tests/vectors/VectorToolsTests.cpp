#include "VectorTools.h"
#include <vector>
#include <catch.hpp>

using std::vector;

TEST_CASE("VectorToolsTests,TestWithIntegers") {
	vector<int> vec;
	insertUnique(vec,0);
	REQUIRE(1 == (int)vec.size());
	REQUIRE(0 == vec[0]);

	insertUnique(vec,0);
	REQUIRE(1 == (int)vec.size());

	insertUnique(vec,1);
	REQUIRE(2 == (int)vec.size());

	for(int i=9;i>0;i--)
		insertUnique(vec,i);
	REQUIRE(10 == (int)vec.size());
	for(unsigned int i=0;i<vec.size()-1;i++)
		CHECK(vec[i] < vec[i+1]);
	for(int i=5;i<12;i++)
		insertUnique(vec,i);
	REQUIRE(12 == (int)vec.size());
}

