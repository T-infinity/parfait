
#include "STL.h"
#include "STLFactory.h"
#include <catch.hpp>

using namespace Parfait;


TEST_CASE("STLTests, Closest"){
    auto stl = STLFactory::getUnitCube();
    stl.scaleToUnitLength();

    STL::SearchSTL searchSTL(stl);
    double distance;
    auto p = searchSTL.getClosestPoint({2,2,2}, distance);
    REQUIRE(p.approxEqual({.5,.5,.5}));
}

TEST_CASE("STLTests, ClosestBugFixCheck"){
    auto stl = STLFactory::getUnitCube();
    stl.scaleToUnitLength();

    STL::SearchSTL searchSTL(stl);
    Parfait::Point<double> p(-1,1,1);
    auto closest = searchSTL.getClosestPoint(p);
    REQUIRE(closest.approxEqual({-.5,.5,.5}));
}

TEST_CASE("STLTests, findDomain"){
    auto stl = STLFactory::getUnitCube();
    stl.scaleToUnitLength();
    auto domain = stl.findDomain();
    REQUIRE(-0.5 == Approx(domain.lo[0]));
    REQUIRE( 0.5 == Approx(domain.hi[0]));
}
