#include <parfait/Facet.h>
#include <catch.hpp>

using namespace Parfait;

TEST_CASE("Facet, Exists"){

    Facet f;
    Point<double> a{0,1,2};
    Point<double> b{3,4,5};
    Point<double> c{6,7,8};

    Facet g(a, b, c);

    REQUIRE( a[0] == g.GetXMin());
    REQUIRE( a[1] == g.GetYMin());
    REQUIRE( a[2] == g.GetZMin());

    REQUIRE( c[0] == g.GetXMax());
    REQUIRE( c[1] == g.GetYMax());
    REQUIRE( c[2] == g.GetZMax());
}

TEST_CASE("Facet, FacetRayIntersection"){

    Facet f({0,0,0}, {.5,0,0}, {0,.5,0});

    Point<double> lo{0.25, 0.25, 1};
    Point<double> hi{0.25, 0.25, -1};

    Point<double> intersect;
    REQUIRE(f.WhereDoesEdgeIntersect(lo, hi, intersect));

    REQUIRE(0.25 == intersect[0]);
    REQUIRE(0.25 == intersect[1]);
    REQUIRE(0.00 == intersect[2]);
}

TEST_CASE("Facet, NormalVector"){
    Facet f({0,0,0}, {.5,0,0}, {0,.5,0});

    auto normal = f.computeNormal();
    REQUIRE(0  == normal[0]);
    REQUIRE(0  == normal[1]);
    REQUIRE(1  == normal[2]);
}

TEST_CASE("Facet, get closest point"){
    Facet f({0,0,0}, {.5,0,0}, {0,.5,0});


    Point<double> p = {0,0,0};
    
    double dist = 0;
    auto closest = f.GetClosestPoint(p, dist);
    REQUIRE(dist == Approx(0));
}
