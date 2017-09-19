#include <catch.hpp>
#include <vector>
#include <parfait/Point.h>
#include <parfait/DualMetrics.h>

TEST_CASE("Add dual metrics tet"){
    std::vector<std::array<int, 2>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
    std::vector<Parfait::Point<double>> xyz = {{0,0,0}, {1,0,0}, {1,1,0}, {0,0,1}};
    std::vector<int> tet = {0,1,2,3};

    DualMetrics dual_metrics(4, 6);
    auto getTet = [&](int tet_id) {
        return tet;
    };
    auto getXyz = [&](int node){
        return xyz[node];
    };
    addTetDualMetrics(getTet, getXyz, 1, edges, dual_metrics);


    for(int edge = 0; edge < edges.size(); edge++) {
        int lnode = edges[edge][0];
        int rnode = edges[edge][1];
        auto& a = dual_metrics.edge_areas[edge];
        if(lnode == 0 and rnode == 1){
            REQUIRE(a[0] == Approx(0.083333));
            REQUIRE(a[1] == Approx(-0.041667));
            REQUIRE(a[2] == Approx(0.041667));
        }
        if(lnode == 1 and rnode == 2){
            REQUIRE(a[0] == Approx(-0.041667));
            REQUIRE(a[1] == Approx(0.083333));
            REQUIRE(a[2] == Approx(0));
        }
        if(lnode == 0 and rnode == 2){
            REQUIRE(a[0] == Approx(0.041667));
            REQUIRE(a[1] == Approx(0.041667));
            REQUIRE(a[2] == Approx(0.041667));
        }
    }
}