#include "node_to_node.h"
#include "imported_ugrid_factory.h"
#include "range_loop.h"
#include "CppUTest/CommandLineTestRunner.h"


TEST_GROUP(NodeToNodeTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(NodeToNodeTests,Exists)
{
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid("grids/6cell.ugrid");

	vector<int> nodeList = buildUniqueNodeList(ugrid);
	vector<vector<int> > n2n = buildNodeToNode(ugrid,nodeList);

	for(int i:range(nodeList))
		LONGS_EQUAL(i,nodeList[i]);

#if 0
	for(int i:range(n2n))
	{
		printf("Node %i connected to: ",i);
		for(int nbr:n2n[i])
			printf("%i ",nbr);
		printf("\n");
	}
#endif
}
