#ifndef ADT_3D_EXTENT_H
#include "Adt3dExtent.h"
#endif

#include "CppUTest/CommandLineTestRunner.h"

using namespace Parfait;

TEST_GROUP(Adt3DExtent) {} ;

TEST(Adt3DExtent, StoreExtent)
{
    Adt3DExtent adt(Extent<double>(Point<double>(0,0,0), Point<double>(1,1,1)));
    Extent<double> store1(Point<double>(.1,.1,.1), Point<double>(.2,.2,.2));
    Extent<double> store2(Point<double>(.7,.7,.7), Point<double>(.8,.8,.8));

    adt.store(10, store1);
    adt.store(11, store2);

    std::vector<int> inside;
    Extent<double> extent(Point<double>(0,0,0), Point<double>(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent<double>(Point<double>(0,0,0), Point<double>(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);
}

TEST(Adt3DExtent, StoreNegativeExtent)
{

    Adt3DExtent adt(Extent<double>(Point<double>(-2,-2,-2), Point<double>(2,2,2)));
    Extent<double> store1(Point<double>(.1,.1,.1), Point<double>(.2,.2,.2));
    Extent<double> store2(Point<double>(.7,.7,.7), Point<double>(.8,.8,.8));

    adt.store(10, store1);
    adt.store(11, store2);

    std::vector<int> inside;
    Extent<double> extent(Point<double>(0,0,0), Point<double>(1,1,1));
    inside = adt.retrieve(extent);

    CHECK_EQUAL(2, inside.size());

    extent = Extent<double>(Point<double>(0,0,0), Point<double>(.5,.5,.5));
    inside = adt.retrieve(extent);
    CHECK_EQUAL(1, inside.size());
    CHECK_EQUAL(10, inside[0]);

    adt.store(13, Extent<double>(Point<double>(-.5,-1,-1), Point<double>(0,0,0)));
    inside = adt.retrieve(Extent<double>(Point<double>(-4,-4,-4), Point<double>(4,4,4)));
    CHECK_EQUAL(3, inside.size());

}
