#include <stdexcept>
#include <iostream>

#ifndef ADT_3D_POINT_H
#include "Adt3dPoint.h"
#endif

#include "CppUTest/CommandLineTestRunner.h"

using namespace Parfait;

TEST_GROUP(Adt3DPoint) {} ;

TEST(Adt3DPoint, StoreNoPoint){

    Adt3DPoint adt({{0,0,0}, {1,1,1}});
    Extent<double> domain({-1,-1,-1}, {2,2,2});
    auto inside = adt.retrieve(domain);
    LONGS_EQUAL(0, inside.size());

}

TEST(Adt3DPoint, StorePoint){
  Adt3DPoint adt({{0,0,0}, {1,1,1}});
    Point<double> point1(.1,.1,.1);
    Point<double> point2(.7,.7,.7);

    adt.store(10, point1);
    adt.store(11, point2);

    std::vector<int> inside;
    Extent<double> extent({0,0,0}, {1,1,1});
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent<double>({0,0,0},{.5,.5,.5});
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);
}

TEST(Adt3DPoint, StoreNegativePoint){

    Adt3DPoint adt(Extent<double>(Point<double>(-2,-2,-2), Point<double>(2,2,2)));
    Point<double> point1(.1,.1,.1);
    Point<double> point2(.7,.7,.7);

    adt.store(10, point1);
    adt.store(11, point2);

    std::vector<int> inside;
    Extent<double> extent(Point<double>(0,0,0), Point<double>(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent<double>(Point<double>(0,0,0), Point<double>(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);

    adt.store(12, Point<double>(-.5,-1,-1));
    inside = adt.retrieve(Extent<double>(Point<double>(-2,-2,-2), Point<double>(2,2,2)));
    CHECK_EQUAL(3, inside.size());

    adt.store(13, Point<double>(-.5,-1,-1));
    inside = adt.retrieve(Extent<double>(Point<double>(-4,-4,-4), Point<double>(4,4,4)));
    CHECK_EQUAL(4, inside.size());
}

TEST(Adt3DPoint, StorePointOutsideDomain)
{
    Adt3DPoint adt(Extent<double>(Point<double>(-2,-2,-2), Point<double>(2,2,2)));
    Point<double> point(5,5,5);

    //CHECK_THROWS(std::exception, adt.store(10, point));
}






