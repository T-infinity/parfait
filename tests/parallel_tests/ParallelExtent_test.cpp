#include "MessagePasser.h"
#include "Extent.h"
#include "ParallelExtent.h"
#include "CppUTest/CommandLineTestRunner.h"

using namespace Parfait;

TEST_GROUP(ParallelExtentBox){};

TEST(ParallelExtentBox, SingleBox){
    Extent<double> e({0,0,0},{1,1,1});
    e.hi[0] = MessagePasser::Rank();
    e.hi[1] = MessagePasser::Rank();
    e.hi[2] = MessagePasser::Rank();

    auto boundingBox = ParallelExtent::getBoundingBox(e);
    DOUBLES_EQUAL(0.0, boundingBox.lo[0], 1.0e-17);
    DOUBLES_EQUAL(0.0, boundingBox.lo[1], 1.0e-17);
    DOUBLES_EQUAL(0.0, boundingBox.lo[2], 1.0e-17);

    int expectedHi = MessagePasser::NumberOfProcesses() - 1;

    DOUBLES_EQUAL(expectedHi, boundingBox.hi[0], 1.0e-17);
    DOUBLES_EQUAL(expectedHi, boundingBox.hi[1], 1.0e-17);
    DOUBLES_EQUAL(expectedHi, boundingBox.hi[2], 1.0e-17);
}

TEST(ParallelExtentBox, VectorOfBoxes){
    std::vector<Extent<int>> boxes;
    int d = MessagePasser::Rank();
    for(int i = 0; i < 100; i++)
        boxes.push_back({{0,0,0},{d,d,d}});

    auto boundingBox = ParallelExtent::getBoundingBox(boxes);

    int expectedHi = MessagePasser::NumberOfProcesses() - 1;
    DOUBLES_EQUAL(0.0, boundingBox.lo[0], 1.0e-17);
    DOUBLES_EQUAL(0.0, boundingBox.lo[1], 1.0e-17);
    DOUBLES_EQUAL(0.0, boundingBox.lo[2], 1.0e-17);
    DOUBLES_EQUAL(expectedHi, boundingBox.hi[0], 1.0e-17);
    DOUBLES_EQUAL(expectedHi, boundingBox.hi[1], 1.0e-17);
    DOUBLES_EQUAL(expectedHi, boundingBox.hi[2], 1.0e-17);
}