#include "adt_builder.h"
#include <vector>
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;
using namespace Parfait;

TEST_GROUP(AdtBuilderTests) {
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
        vector<double> nodes;
    };

    MockMesh mesh;
    void setup() {
        mesh.nodes = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
    }
    void teardown() {}
};

TEST(AdtBuilderTests, Exists) {
    // put all cells in mesh into an ADT
    Adt3DExtent adt = AdtBuilder::putCellsInAdt(mesh);

    // get all cells that overlap with the origin
    // (should be just the one cell of the mock mesh)
    auto cellIds = adt.retrieve(Extent({0, 0, 0}, {0, 0, 0}));
    LONGS_EQUAL(1, cellIds.size());
    LONGS_EQUAL(0, cellIds[0]);
}
