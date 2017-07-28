#include <catch.hpp>
#include <parfait/Extent.h>

using namespace Parfait;

TEST_CASE("Extent, Constructor"){

    Point<double> lo(0,0,0), hi(1,1,1);
    Extent<double> v(lo, hi);

    REQUIRE(v.lo[0] == 0);
    REQUIRE(v.lo[1] == 0);
    REQUIRE(v.lo[2] == 0);

    REQUIRE(v.hi[0] == 1);
    REQUIRE(v.hi[1] == 1);
    REQUIRE(v.hi[2] == 1);

}

TEST_CASE("stl like functions"){
    Extent<int> extent{{0,0,0},{1,1,1}};
    REQUIRE(extent.lo.data() == extent.data());
}

TEST_CASE("Extent, PointerConstructors"){

    double d_lo[3] = {0.1,0.1,0.1};
    double d_hi[3] = {1.1,1.1,1.1};

    Extent<double> d_extent(d_lo, d_hi);
    REQUIRE(0.1 == d_extent.lo[0]);
    REQUIRE(0.1 == d_extent.lo[1]);
    REQUIRE(0.1 == d_extent.lo[2]);

    REQUIRE(1.1 == d_extent.hi[0]);
    REQUIRE(1.1 == d_extent.hi[1]);
    REQUIRE(1.1 == d_extent.hi[2]);

    double extent_in[6] = {2.2,3.3,4.4, 5.5, 6.6, 7.7};
    Extent<double> e_extent(extent_in);
    REQUIRE(2.2 == e_extent.lo[0]);
    REQUIRE(3.3 == e_extent.lo[1]);
    REQUIRE(4.4 == e_extent.lo[2]);

    REQUIRE(5.5 == e_extent.hi[0]);
    REQUIRE(6.6 == e_extent.hi[1]);
    REQUIRE(7.7 == e_extent.hi[2]);

}

TEST_CASE("Extent, containsPoint"){

    Point<double> lo(0,0,0), hi(1,1,1);
    Extent<double> v(lo, hi);

    Point<double> p(0.25, 0.25, 0.25);
    REQUIRE(v.contains(p));

    p = Point<double>(-1.0, 0.25, 0.0);
    REQUIRE(not v.contains(p));
}

TEST_CASE("Extent, containsExtent"){
    Extent<double> box1({0,0,0}, {1,1,1});
    REQUIRE(box1.contains(Extent<double>({0.1, 0.1, 0.1}, {0.2,0.2, 0.2})));

    Extent<double> box2({-1, -1, -1}, {-0.5, -0.5, 0.5});
    REQUIRE(not box2.contains(box1));
}

TEST_CASE("Extent, Length") {
    Extent<double> e({0,0,0}, {1,1,1});
    REQUIRE(0 == e.lo[0]);
    REQUIRE(0 == e.lo[1]);
    REQUIRE(0 == e.lo[2]);

    REQUIRE(1 == e.hi[0]);
    REQUIRE(1 == e.hi[1]);
    REQUIRE(1 == e.hi[2]);
}

TEST_CASE("Extent,resize"){
    Extent<double> e{{0,0,0},{1,1,1}};
    e.resize(2,4,8);

    REQUIRE(-.5  == e.lo[0]);
    REQUIRE(-1.5 == e.lo[1]);
    REQUIRE(-3.5 == e.lo[2]);
    REQUIRE(1.5 == e.hi[0]);
    REQUIRE(2.5 == e.hi[1]);
    REQUIRE(4.5 == e.hi[2]);
}

TEST_CASE("Extent,makeIsotropic"){
    Extent<double> e{{0,0,0},{2,1,0}};
    e.makeIsotropic();
    REQUIRE(0   == e.lo[0]);
    REQUIRE(-.5 == e.lo[1]);
    REQUIRE(-1  == e.lo[2]);
    REQUIRE(2   == e.hi[0]);
    REQUIRE(1.5 == e.hi[1]);
    REQUIRE(1   == e.hi[2]);
}
