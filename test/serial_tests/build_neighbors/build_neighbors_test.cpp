#include <cart_mesh.h>
#include "build_neighbors.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(BuildNeighbors){
    CartMesh cartMesh = {{0,0,0},{2,1,1},2,1,1};
};

TEST(BuildNeighbors, Exists){
    std::vector<std::vector<int>> neighbors = buildNeighbors(cartMesh);
    LONGS_EQUAL(2, neighbors.size());
    LONGS_EQUAL(6, neighbors[0].size());
    LONGS_EQUAL(6, neighbors[1].size());

    LONGS_EQUAL(-1, neighbors[0][0]);
    LONGS_EQUAL(1, neighbors[0][2]);
    LONGS_EQUAL(-1, neighbors[1][0]);
    LONGS_EQUAL(0, neighbors[1][4]);
}