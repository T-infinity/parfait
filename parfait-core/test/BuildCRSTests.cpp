#include <catch.hpp>
#include <CRS.h>

TEST_CASE("Build CRS"){
    std::vector<std::vector<int>> graph = {
        {0,1,2}, 
        {0,1,2,3,4},
        {0,1,2,3,6},
        {1,2,3,4,6},
        {1,3,4,5},
        {4,5,6},
        {2,3,5,6}
    };

    Parfait::CRS crs(graph);

    REQUIRE(8 == crs.ia.size());
    REQUIRE(0 == crs.ia[0]);
    REQUIRE(3 == crs.ia[1]);
    REQUIRE(6 == crs.ja[12]);
    REQUIRE(20 == crs.iau[4]);
}
