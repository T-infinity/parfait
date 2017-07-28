#include <catch.hpp>
#include <parfait/CartBlock.h>
#include <parfait/NodeToNode.h>

using namespace Parfait;

class n2nMockMesh{
public:
    int numberOfNodes() {return 4;}
    int numberOfCells() {return 1;}
    int numberOfNodesInCell(int i) {return 4;}
    void getNodesInCell(int i,int* cell) {
        cell[0] = 0;
        cell[1] = 1;
        cell[2] = 2;
        cell[3] = 3;
    }
};

TEST_CASE("Build Node To Node from a MeshTemplate"){
    n2nMockMesh mesh;
    NodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
    auto n2n = nodeBuilder.buildNodeToNodeConnectivity();

    REQUIRE(mesh.numberOfNodes() == n2n.size());
    REQUIRE(3 == n2n[0].size());
    REQUIRE(1 == n2n[0][0]);
    REQUIRE(2 == n2n[0][1]);
    REQUIRE(3 == n2n[0][2]);
}
