#include <catch.hpp>
#include <parfait/Point.h>
#include <parfait/Normals.h>

TEST_CASE("Good Normal average"){
    Parfait::Point<double> n1 = {0,1,0};
    Parfait::Point<double> n2 = {1,0,0};

    auto n = Parfait::Normals::average({n1, n2});
    REQUIRE(n[0] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[1] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[2] == 0);
}

TEST_CASE("Good Normal with redundant normals"){
    Parfait::Point<double> n1 = {0,1,0};
    Parfait::Point<double> n2 = {1,0,0};

    auto n = Parfait::Normals::goodNormal({n1, n1, n1, n2});
    REQUIRE(n[0] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[1] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[2] == 0);
}

TEST_CASE("Good Normal, extreme case"){
    Parfait::Point<double> n1 = {1,0,0};
    Parfait::Point<double> n2 = {-1,0,0};
    Parfait::Point<double> n3 = {0,1,0};
    auto n = Parfait::Normals::goodNormal({n1, n2, n3});
    REQUIRE(n[0] == Approx(0));
    REQUIRE(n[1] == Approx(1));
    REQUIRE(n[2] == Approx(0));
}

