#include <catch.hpp>
#include "CartMesh.h"
#include "BuildNeighbors.h"

using namespace Parfait;

TEST_CASE("BuildNeighbors, Exists"){
CartMesh cartMesh = {{0,0,0},{2,1,1},2,1,1};
    std::vector<std::vector<int>> neighbors = buildNeighbors(cartMesh);
    REQUIRE(2 == neighbors.size());
    REQUIRE(6 == neighbors[0].size());
    REQUIRE(6 == neighbors[1].size());

    REQUIRE(-1 == neighbors[0][0]);
    REQUIRE(1 == neighbors[0][2]);
    REQUIRE(-1 == neighbors[1][0]);
    REQUIRE(0 == neighbors[1][4]);
}