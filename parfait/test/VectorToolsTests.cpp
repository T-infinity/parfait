#include <vector>
#include <catch.hpp>
#include <parfait/VectorTools.h>

using std::vector;

TEST_CASE("VectorToolsTests,TestWithIntegers") {
	vector<int> vec;
	Parfait::insertUnique(vec,0);
	REQUIRE(1 == (int)vec.size());
	REQUIRE(0 == vec[0]);

	Parfait::insertUnique(vec,0);
	REQUIRE(1 == (int)vec.size());

	Parfait::insertUnique(vec,1);
	REQUIRE(2 == (int)vec.size());

	for(int i=9;i>0;i--)
		Parfait::insertUnique(vec,i);
	REQUIRE(10 == (int)vec.size());
	for(unsigned int i=0;i<vec.size()-1;i++)
		CHECK(vec[i] < vec[i+1]);
	for(int i=5;i<12;i++)
		Parfait::insertUnique(vec,i);
	REQUIRE(12 == (int)vec.size());
}

