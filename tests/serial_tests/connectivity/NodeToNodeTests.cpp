#include <ImportedUgrid.h>
#include "NodeToNode.h"
#include <catch.hpp>
#include "ParallelMesh.h"
#include <PartitionableMesh.h>
using namespace Parfait;

using namespace std;

TEST_CASE("NodeToNodeTests,Exists") {
    vector<double> xyz {0,0,0,
                        1,0,0,
                        1,1,0,
                        0,0,1};
    vector<int> tets {0,1,2,3};
    ImportedUgrid one_tet_mesh(xyz,{},{},tets,{},{},{},{},{});

    std::shared_ptr<MeshConnectivity> connectivity = std::make_shared<MeshConnectivity>();
    std::shared_ptr<MeshParallelMetaData> metaData = std::make_shared<MeshParallelMetaData>();
    connectivity->tets = tets;
    metaData->xyz = xyz;
    auto mesh = std::make_shared<ParallelMesh>(connectivity, metaData);
    PartitionableMesh partitionableMesh(mesh);
    NodeToNodeBuilder<decltype(partitionableMesh)> builder(partitionableMesh);
    auto n2n = builder.buildNodeToNodeConnectivity();

    REQUIRE(4==n2n.size());

    // check connectivity for node 0
    REQUIRE(3==n2n[0].size());
    REQUIRE(1==n2n[0][0]);
    REQUIRE(2==n2n[0][1]);
    REQUIRE(3==n2n[0][2]);
    // check connectivity for node 1
    REQUIRE(3==n2n[1].size());
    REQUIRE(0==n2n[1][0]);
    REQUIRE(2==n2n[1][1]);
    REQUIRE(3==n2n[1][2]);
    // check connectivity for node 2
    REQUIRE(3==n2n[2].size());
    REQUIRE(0==n2n[2][0]);
    REQUIRE(1==n2n[2][1]);
    REQUIRE(3==n2n[2][2]);
    // check connectivity for node 3
    REQUIRE(3==n2n[3].size());
    REQUIRE(0==n2n[3][0]);
    REQUIRE(1==n2n[3][1]);
    REQUIRE(2==n2n[3][2]);
}
