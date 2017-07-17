#include <catch.hpp>
#include <EdgeFieldLeastSquaresGradient.h>

TEST_CASE("Edge collection least squares gradient of more than 1 variable"){
    std::vector<std::array<int, 2>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
    std::vector<Parfait::Point<double>> points = {{0,0,0},{1,0,0},{0,1,0},{0,0,1}};
    auto getXyz = [&](int i){return points[i];};
    auto getEdge = [&](int edge){return edges[edge];};

    std::vector<std::array<double, 2>> var(points.size());
    for(int i = 0; i < points.size(); i++){
        auto p = getXyz(i);
        var[i][0] = 5*p[0] + 1*p[1] + 3*p[2];
        var[i][1] = 5.5*p[0] + 1.1*p[1] + 3.3*p[2];
    }
    auto grad = calculateNodeGradientsFromEdges(getEdge, edges.size(), getXyz, points.size(), var);

    REQUIRE(grad.size() == points.size());
    for(auto& g : grad){
        REQUIRE(g[0][0] == Approx(5));
        REQUIRE(g[0][1] == Approx(1));
        REQUIRE(g[0][2] == Approx(3));

        REQUIRE(g[1][0] == Approx(5.5));
        REQUIRE(g[1][1] == Approx(1.1));
        REQUIRE(g[1][2] == Approx(3.3));
    }
}

