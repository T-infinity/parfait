#include <catch.hpp>
#include <parfait/LeastSquaresGradient.h>

TEST_CASE("Gram-Schmidt weight calculation, unit directions, check diagon"){
    std::vector<Parfait::Point<double>> points = {{1,0,0},{0,1,0},{0,0,1}};
    Parfait::Point<double> center = {0,0,0};
    auto gs = calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx(1));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(0));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(1));
}

TEST_CASE("Gram-Schmidt weight calculation, x and z off diag"){
    std::vector<Parfait::Point<double>> points = {{0,0,0},{1,1,0},{0,0,1}};
    Parfait::Point<double> center = {1,0,0};
    auto gs = calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx( 1.41421));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(-0.70711));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(0.70711));
}

TEST_CASE("Least Squares gradient calculation"){
    std::vector<Parfait::Point<double>> points = {{1,0,0},{0,1,0},{0,0,1}};
    Parfait::Point<double> center = {0,0,0};
    std::vector<double> u = {5, 1, 3}; // 5x + y + 3z

    auto gs= calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx(1));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(0));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(1));

    double u0 = 0;
    auto grad = calcLeastSquaresGradient(points, center, gs, u, u0);
    REQUIRE(grad[0] == Approx(5));
    REQUIRE(grad[1] == Approx(1));
    REQUIRE(grad[2] == Approx(3));
}