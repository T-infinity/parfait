#include <Extent.h>
#include "ExtentMeshWrapper.h"
#include "catch.hpp"

using namespace Parfait;

TEST_CASE("To turn extent boxes into a mesh that can be visualized"){
    Extent<double> e {{0,0,0},{1,1,1}};
    ExtentMeshWrapper extentMesh;
    extentMesh.addExtent(e);
    REQUIRE(8 == extentMesh.numberOfNodes());
    REQUIRE(1 == extentMesh.numberOfCells());
    REQUIRE(8 == extentMesh.numberOfNodesInCell(0));
    auto p = extentMesh.getNode(1);
    REQUIRE(p[0] == 1);
    REQUIRE(p[1] == 0);
    REQUIRE(p[2] == 0);

    extentMesh.addExtent(e);
    REQUIRE(16 == extentMesh.numberOfNodes());
    REQUIRE(2 == extentMesh.numberOfCells());
    p = extentMesh.getNode(13);
    REQUIRE(p[0] == 1);
    REQUIRE(p[1] == 0);
    REQUIRE(p[2] == 1);

    auto vertexIds = extentMesh.getNodesInCell(1);
    REQUIRE(8 == vertexIds[0]);
    REQUIRE(9 == vertexIds[1]);
    REQUIRE(10 == vertexIds[2]);
    REQUIRE(11 == vertexIds[3]);
    REQUIRE(12 == vertexIds[4]);
    REQUIRE(13 == vertexIds[5]);
    REQUIRE(14 == vertexIds[6]);
    REQUIRE(15 == vertexIds[7]);
}

