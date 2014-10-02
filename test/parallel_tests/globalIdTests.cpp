#include "CppUTest/CommandLineTestRunner.h"
#include "parfait.h"

using namespace MessagePasser;

class MockMesh
{
	public:
		MockMesh(int n):nnodes(n){}
		int numberOfNodes(){return nnodes;}
	private:
		int nnodes;
};

TEST_GROUP(globalIdTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(globalIdTests,Exists)
{
	MockMesh mesh(5);
	GlobalIdTranslator tran(mesh);

	LONGS_EQUAL(Rank()*5,tran.globalNodeId(0));
	LONGS_EQUAL(Rank()*5+2,tran.globalNodeId(2));
	
	LONGS_EQUAL(0,tran.localNodeId(Rank()*5));
	LONGS_EQUAL(2,tran.localNodeId(Rank()*5+2));
}
