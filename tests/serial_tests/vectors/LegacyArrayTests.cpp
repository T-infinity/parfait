#include "LegacyArray.h"
#include "RangeLoop.h"
#include <catch.hpp>

TEST_CASE("LegacyArrayTests,Exists") {
	int a[6] = {0,1,2,3,4,5};
	LegacyArray<int> b(a,6);

	REQUIRE(6 == b.size());
	//auto beginIterator = b.begin();
	//auto endIterator   = b.end();

	for(int i:range(b))
		REQUIRE(a[i] == b[i]);

	for(int i:b)
		REQUIRE(a[i] == i);
	b[0] = 4;
	REQUIRE(4 == b[0]);
}
