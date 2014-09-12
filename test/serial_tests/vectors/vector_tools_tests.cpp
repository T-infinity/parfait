#include "CppUTest/CommandLineTestRunner.h"
#include "vector_tools.h"
#include <vector>

using std::vector;

TEST_GROUP(VectorToolsTests)
{
};

TEST(VectorToolsTests,TestWithIntegers)
{
	vector<int> vec;
	// insert a zero and make sure its there
	insertUnique(vec,0);
	LONGS_EQUAL(1,(int)vec.size());
	LONGS_EQUAL(0,vec[0]);

	// insert another zero and make sure there's only one
	insertUnique(vec,0);
	LONGS_EQUAL(1,(int)vec.size());

	// insert a 1
	insertUnique(vec,1);
	LONGS_EQUAL(2,(int)vec.size());

	// insert 9-0, and check that they end up in order
	for(int i=9;i>0;i--)
		insertUnique(vec,i);
	LONGS_EQUAL(10,(int)vec.size());
	for(int i=0;i<vec.size()-1;i++)
		CHECK(vec[i] < vec[i+1]);
	// insert 5-11, and check that only 2 are added
	for(int i=5;i<12;i++)
		insertUnique(vec,i);
	LONGS_EQUAL(12,(int)vec.size());
}

