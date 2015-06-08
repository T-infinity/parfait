#include "edge_map.h"
#include "cart_mesh.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;

TEST_GROUP(EdgeMap){};

TEST(EdgeMap, CreateUniqueEdges){
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);

    EdgeMap<CartMesh> edgeMap(cartMesh);

    LONGS_EQUAL(54, edgeMap.edges.size());

    CHECK(edgeMap.getEdgeId({{1,2}}) == edgeMap.getEdgeId({{2,1}}));
}

