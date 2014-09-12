#include <stdexcept>
#include <iostream>

#ifndef ADT_3D_POINT_H
#include "adt_3d_point.h"
#endif

#include "CppUTest/CommandLineTestRunner.h"


TEST_GROUP(Adt3DPoint) {} ;

TEST(Adt3DPoint, StorePoint){
    Adt3DPoint adt(Extent(Point(0,0,0), Point(1,1,1)));
    Point point1(.1,.1,.1);
    Point point2(.7,.7,.7);

    adt.store(10, point1);
    adt.store(11, point2);

    std::vector<int> inside;
    Extent extent(Point(0,0,0), Point(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent(Point(0,0,0), Point(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);
}

TEST(Adt3DPoint, StoreNegativePoint){

    Adt3DPoint adt(Extent(Point(-2,-2,-2), Point(2,2,2)));
    Point point1(.1,.1,.1);
    Point point2(.7,.7,.7);

    adt.store(10, point1);
    adt.store(11, point2);

    std::vector<int> inside;
    Extent extent(Point(0,0,0), Point(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent(Point(0,0,0), Point(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);

    adt.store(12, Point(-.5,-1,-1));
    inside = adt.retrieve(Extent(Point(-2,-2,-2), Point(2,2,2)));
    CHECK_EQUAL(3, inside.size());

    adt.store(13, Point(-.5,-1,-1));
    inside = adt.retrieve(Extent(Point(-4,-4,-4), Point(4,4,4)));
    CHECK_EQUAL(4, inside.size());
}

TEST(Adt3DPoint, StorePointOutsideDomain)
{
    Adt3DPoint adt(Extent(Point(-2,-2,-2), Point(2,2,2)));
    Point point(5,5,5);

    //bool caught = false;
    //try {
    //    adt.store(10, point);
    //}
    //catch(std::domain_error &e){
    //    printf("\n\n\nCaught Exception!");
    //    std::cout << e.what() ;
    //    caught = true;
    //}
    //CHECK_EQUAL(true, caught);
}
