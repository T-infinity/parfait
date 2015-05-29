#include "legacy_array.h"
#include "range_loop.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(LegacyArrayTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(LegacyArrayTests,Exists)
{
	int a[6] = {0,1,2,3,4,5};
	LegacyArray<int> b(a,6);

	LONGS_EQUAL(6,b.size());
	auto beginIterator = b.begin();
	auto endIterator   = b.end();

	for(int i:range(b))
		LONGS_EQUAL(a[i],b[i]);

	for(int i:b)
		LONGS_EQUAL(a[i],i);
	b[0] = 4;
	LONGS_EQUAL(4,b[0]);	
}
