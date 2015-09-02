#include "MessagePasser.h"
#include "Extent.h"
#include "ParallelExtent.h"
#include <catch.hpp>

using namespace Parfait;


TEST_CASE("SingleBox"){
    Extent<double> e({0,0,0},{1,1,1});
    e.hi[0] = MessagePasser::Rank();
    e.hi[1] = MessagePasser::Rank();
    e.hi[2] = MessagePasser::Rank();

    auto boundingBox = ParallelExtent::getBoundingBox(e);
    REQUIRE(0.0 == Approx(boundingBox.lo[0]));
    REQUIRE(0.0 == Approx(boundingBox.lo[1]));
    REQUIRE(0.0 == Approx(boundingBox.lo[2]));

    int expectedHi = MessagePasser::NumberOfProcesses() - 1;

    REQUIRE(expectedHi == Approx(boundingBox.hi[0]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[1]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[2]));
}

TEST_CASE("VectorOfBoxes"){
    std::vector<Extent<int>> boxes;
    int d = MessagePasser::Rank();
    for(int i = 0; i < 100; i++)
        boxes.push_back({{0,0,0},{d,d,d}});

    auto boundingBox = ParallelExtent::getBoundingBox(boxes);

    int expectedHi = MessagePasser::NumberOfProcesses() - 1;
    REQUIRE(0.0 == Approx(boundingBox.lo[0]));
    REQUIRE(0.0 == Approx(boundingBox.lo[1]));
    REQUIRE(0.0 == Approx(boundingBox.lo[2]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[0]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[1]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[2]));
}
