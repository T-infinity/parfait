#include "facet.h"
#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;
TEST_GROUP(Facet){};

TEST(Facet, Exists){

    Facet f;
    Point a{0,1,2};
    Point b{3,4,5};
    Point c{6,7,8};

    Facet g(a, b, c);

    CHECK( a[0] == g.GetXMin());
    CHECK( a[1] == g.GetYMin());
    CHECK( a[2] == g.GetZMin());

    CHECK( c[0] == g.GetXMax());
    CHECK( c[1] == g.GetYMax());
    CHECK( c[2] == g.GetZMax());
}

TEST(Facet, FacetRayIntersection){

    Facet f({0,0,0},
            {.5,0,0},
            {0,.5,0});

    Point lo{0.25, 0.25, 1};
    Point hi{0.25, 0.25, -1};

    Point intersect;
    CHECK(f.WhereDoesEdgeIntersect(lo, hi, intersect));

    CHECK(0.25 == intersect[0]);
    CHECK(0.25 == intersect[1]);
    CHECK(0.00 == intersect[2]);
}

TEST(Facet, NormalVector){
    Facet f({0,0,0},
    {.5,0,0},
    {0,.5,0});

    Point normal = f.computeNormal();
    CHECK(0  == normal[0]);
    CHECK(0  == normal[1]);
    CHECK(1  == normal[2]);
}
