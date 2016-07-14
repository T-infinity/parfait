#include "EdgeMap.h"
#include "CartMesh.h"
#include <catch.hpp>

using namespace Parfait;

TEST_CASE("EdgeMap, CreateUniqueEdges"){
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);

    EdgeMap<CartMesh> edgeMap(cartMesh);

    REQUIRE(54 == edgeMap.edges.size());

    REQUIRE(edgeMap.getEdgeId({{1,2}}) == edgeMap.getEdgeId({{2,1}}));
}

