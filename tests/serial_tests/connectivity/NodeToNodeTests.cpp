#include "NodeToNode.h"
#include "ImportedUgrid.h"
#include "CppUTest/CommandLineTestRunner.h"
using namespace Parfait;

using namespace std;

TEST_GROUP(NodeToNodeTests) {
};

TEST(NodeToNodeTests,Exists)
{
    vector<double> xyz {0,0,0,
                        1,0,0,
                        1,1,0,
                        0,0,1};
    vector<int> tets {0,1,2,3};
    ImportedUgrid one_tet_mesh(xyz,{},{},tets,{},{},{},{},{},{},{});

    NodeToNodeBuilder<decltype(one_tet_mesh)> builder(one_tet_mesh);
    auto n2n = builder.buildNodeToNodeConnectivity();

    LONGS_EQUAL(4,n2n.size());

    // check connectivity for node 0
    LONGS_EQUAL(3,n2n[0].size());
    LONGS_EQUAL(1,n2n[0][0]);
    LONGS_EQUAL(2,n2n[0][1]);
    LONGS_EQUAL(3,n2n[0][2]);
    // check connectivity for node 1
    LONGS_EQUAL(3,n2n[1].size());
    LONGS_EQUAL(0,n2n[1][0]);
    LONGS_EQUAL(2,n2n[1][1]);
    LONGS_EQUAL(3,n2n[1][2]);
    // check connectivity for node 2
    LONGS_EQUAL(3,n2n[2].size());
    LONGS_EQUAL(0,n2n[2][0]);
    LONGS_EQUAL(1,n2n[2][1]);
    LONGS_EQUAL(3,n2n[2][2]);
    // check connectivity for node 3
    LONGS_EQUAL(3,n2n[3].size());
    LONGS_EQUAL(0,n2n[3][0]);
    LONGS_EQUAL(1,n2n[3][1]);
    LONGS_EQUAL(2,n2n[3][2]);
}
