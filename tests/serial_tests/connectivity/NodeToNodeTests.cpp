#include "node_to_node.h"
#include "imported_ugrid_factory.h"
#include "range_loop.h"
#include "CppUTest/CommandLineTestRunner.h"
using namespace Parfait;

TEST_GROUP(NodeToNodeTests) {
};

TEST(NodeToNodeTests,Exists)
{
    printf("%s\n",SIX_CELL_TEST_GRID);
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid(SIX_CELL_TEST_GRID);

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
