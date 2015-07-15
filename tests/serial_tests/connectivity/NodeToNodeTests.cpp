#include "NodeToNode.h"
#include "ImportedUgridFactory.h"
#include "RangeLoop.h"
#include "CppUTest/CommandLineTestRunner.h"
using namespace Parfait;

TEST_GROUP(NodeToNodeTests) {
};

TEST(NodeToNodeTests,Exists)
{
    printf("%s\n",SIX_CELL_TEST_GRID);
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid(SIX_CELL_TEST_GRID);

	auto nodeList = buildUniqueNodeList(ugrid);
	auto n2n = buildNodeToNode(ugrid,nodeList);

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
