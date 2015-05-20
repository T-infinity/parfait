#ifndef ADT_3D_EXTENT_H
#include "adt_3d_extent.h"
#endif

#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(Adt3DExtent) {} ;

TEST(Adt3DExtent, StoreExtent)
{
    Adt3DExtent adt(Extent(Point(0,0,0), Point(1,1,1)));
    Extent store1(Point(.1,.1,.1), Point(.2,.2,.2));
    Extent store2(Point(.7,.7,.7), Point(.8,.8,.8));

    adt.store(10, store1);
    adt.store(11, store2);

    std::vector<int> inside;
    Extent extent(Point(0,0,0), Point(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent(Point(0,0,0), Point(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);
}

TEST(Adt3DExtent, StoreNegativeExtent)
{

    Adt3DExtent adt(Extent(Point(-2,-2,-2), Point(2,2,2)));
    Extent store1(Point(.1,.1,.1), Point(.2,.2,.2));
    Extent store2(Point(.7,.7,.7), Point(.8,.8,.8));

    adt.store(10, store1);
    adt.store(11, store2);

    std::vector<int> inside;
    Extent extent(Point(0,0,0), Point(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent(Point(0,0,0), Point(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);

    adt.store(13, Extent(Point(-.5,-1,-1), Point(0,0,0)));
    inside = adt.retrieve(Extent(Point(-4,-4,-4), Point(4,4,4)));
    CHECK_EQUAL(3, inside.size());

}
