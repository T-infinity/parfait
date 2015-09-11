#include <catch.hpp>
#include "AdtBuilder.h"
#include <vector>

using std::vector;
using namespace Parfait;

class MockMesh {
public:
    int numberOfNodes() { return 4; }
    int numberOfCells() { return 1; }
    int numberOfNodesInCell(int cellId) { return 4; }
    std::vector<int> getNodesInCell(int cellId) const {
        std::vector<int> nodeIds(4);
        nodeIds[0] = 0;
        nodeIds[1] = 1;
        nodeIds[2] = 2;
        nodeIds[3] = 3;
        return nodeIds;
    }
    void getNode(int nodeId, double p[3]) const {
        p[0] = nodes[3 * nodeId + 0];
        p[1] = nodes[3 * nodeId + 1];
        p[2] = nodes[3 * nodeId + 2];
    }
    Point<double> getNode(int nodeId) const {
        Point<double> p;
        getNode(nodeId,p.data());
        return p;
    }
    vector<double> nodes;
public:
    MockMesh() {
        nodes = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
    }
};

TEST_CASE("AdtBuilderTests, Exists") {
    MockMesh mesh;
    Adt3DExtent adt = AdtBuilder::putCellsInAdt(mesh);

    auto cellIds = adt.retrieve(Extent<double>({0, 0, 0}, {0, 0, 0}));
    REQUIRE(1 == cellIds.size());
    REQUIRE(0 == cellIds[0]);
}
