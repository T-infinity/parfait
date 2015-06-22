#ifndef __EXTENT_H
#include "extent.h"
#endif
#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;
TEST_GROUP(Extent){

};
double tol = 1.0e-14;

TEST(Extent, Constructor){

    Point<double> lo(0,0,0), hi(1,1,1);
    Extent<double> v(lo, hi);

    CHECK_EQUAL(v.lo[0], 0);
    CHECK_EQUAL(v.lo[1], 0);
    CHECK_EQUAL(v.lo[2], 0);

    CHECK_EQUAL(v.hi[0], 1);
    CHECK_EQUAL(v.hi[1], 1);
    CHECK_EQUAL(v.hi[2], 1);

    Extent<double> w;
    CHECK_EQUAL(w.lo[0], 0);
    CHECK_EQUAL(w.lo[1], 0);
    CHECK_EQUAL(w.lo[2], 0);

    CHECK_EQUAL(w.hi[0], 0);
    CHECK_EQUAL(w.hi[1], 0);
    CHECK_EQUAL(w.hi[2], 0);
}

TEST(Extent, PointerConstructors){

    double d_lo[3] = {0.1,0.1,0.1};
    double d_hi[3] = {1.1,1.1,1.1};

    Extent<double> d_extent(d_lo, d_hi);
    DOUBLES_EQUAL(0.1, d_extent.lo[0], tol);
    DOUBLES_EQUAL(0.1, d_extent.lo[1], tol);
    DOUBLES_EQUAL(0.1, d_extent.lo[2], tol);

    DOUBLES_EQUAL(1.1, d_extent.hi[0], tol);
    DOUBLES_EQUAL(1.1, d_extent.hi[1], tol);
    DOUBLES_EQUAL(1.1, d_extent.hi[2], tol);

    double extent_in[6] = {2.2,3.3,4.4, 5.5, 6.6, 7.7};
    Extent<double> e_extent(extent_in);
    DOUBLES_EQUAL(2.2, e_extent.lo[0], tol);
    DOUBLES_EQUAL(3.3, e_extent.lo[1], tol);
    DOUBLES_EQUAL(4.4, e_extent.lo[2], tol);

    DOUBLES_EQUAL(5.5, e_extent.hi[0], tol);
    DOUBLES_EQUAL(6.6, e_extent.hi[1], tol);
    DOUBLES_EQUAL(7.7, e_extent.hi[2], tol);

}

TEST(Extent, containsPoint){

    Point<double> lo(0,0,0), hi(1,1,1);
    Extent<double> v(lo, hi);

    Point<double> p(0.25, 0.25, 0.25);
    CHECK_EQUAL(true, v.contains(p));

    p = Point<double>(-1.0, 0.25, 0.0);
    CHECK_EQUAL(false, v.contains(p));
}

TEST(Extent, containsExtent){
    Extent<double> box1({0,0,0}, {1,1,1});
    CHECK_EQUAL(true, box1.contains(Extent<double>({0.1, 0.1, 0.1}, {0.2,0.2, 0.2})));

    Extent<double> box2({-1, -1, -1}, {-0.5, -0.5, 0.5});
    CHECK_EQUAL(false, box2.contains(box1));
}

TEST(Extent, Length)
{
    Extent<double> e({0,0,0}, {1,1,1});
    DOUBLES_EQUAL(0, e.lo[0], tol);
    DOUBLES_EQUAL(0, e.lo[1], tol);
    DOUBLES_EQUAL(0, e.lo[2], tol);

    DOUBLES_EQUAL(1, e.hi[0], tol);
    DOUBLES_EQUAL(1, e.hi[1], tol);
    DOUBLES_EQUAL(1, e.hi[2], tol);
}

TEST(Extent,resize){
    Extent<double> e{{0,0,0},{1,1,1}};
    e.resize(2,4,8);

    DOUBLES_EQUAL(-.5,e.lo[0],tol);
    DOUBLES_EQUAL(-1.5,e.lo[1],tol);
    DOUBLES_EQUAL(-3.5,e.lo[2],tol);
    DOUBLES_EQUAL(1.5,e.hi[0],tol);
    DOUBLES_EQUAL(2.5,e.hi[1],tol);
    DOUBLES_EQUAL(4.5,e.hi[2],tol);
}

TEST(Extent,makeIsotropic){
    Extent<double> e{{0,0,0},{2,1,0}};
    e.makeIsotropic();
    DOUBLES_EQUAL(0, e.lo[0],tol);
    DOUBLES_EQUAL(-.5, e.lo[1],tol);
    DOUBLES_EQUAL(-.5, e.lo[2],tol);
    DOUBLES_EQUAL(2, e.hi[0],tol);
    DOUBLES_EQUAL(1.5, e.hi[1],tol);
    DOUBLES_EQUAL(1.5, e.hi[2],tol);
}

