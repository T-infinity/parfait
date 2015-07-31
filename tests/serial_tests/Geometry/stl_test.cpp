
#include "STL.h"
#include "STLFactory.h"
#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;

TEST_GROUP(STLTests){
    STL::STL stl;
    void setup(){
        stl = STLFactory::getUnitCube();
        stl.scaleToUnitLength();
    }
};

TEST(STLTests, Closest){

    STL::SearchSTL searchSTL(stl);
    double distance;
    auto p = searchSTL.getClosestPoint({2,2,2}, distance);
    CHECK(p.approxEqual({.5,.5,.5}));
}

TEST(STLTests, ClosestBugFixCheck){

    STL::SearchSTL searchSTL(stl);
    Parfait::Point<double> p(-1,1,1);
    auto closest = searchSTL.getClosestPoint(p);
    CHECK(closest.approxEqual({-.5,.5,.5}));
}

TEST(STLTests, findDomain){
    auto domain = stl.findDomain();
    DOUBLES_EQUAL(-0.5, domain.lo[0], 1.0e-14);
    DOUBLES_EQUAL( 0.5, domain.hi[0], 1.0e-14);
}
