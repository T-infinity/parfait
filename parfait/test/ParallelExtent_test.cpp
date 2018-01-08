#include <MessagePasser/MessagePasser.h>
#include <parfait/Extent.h>
#include <parfait/ParallelExtent.h>
#include <catch.hpp>

using namespace Parfait;


TEST_CASE("SingleBox"){
    MessagePasser mp;
    Extent<double> e({0,0,0},{1,1,1});
    e.hi[0] = mp.Rank();
    e.hi[1] = mp.Rank();
    e.hi[2] = mp.Rank();

    auto boundingBox = ParallelExtent::getBoundingBox(mp,e);
    REQUIRE(0.0 == Approx(boundingBox.lo[0]));
    REQUIRE(0.0 == Approx(boundingBox.lo[1]));
    REQUIRE(0.0 == Approx(boundingBox.lo[2]));

    int expectedHi = mp.NumberOfProcesses() - 1;

    REQUIRE(expectedHi == Approx(boundingBox.hi[0]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[1]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[2]));
}

TEST_CASE("VectorOfBoxes"){
    MessagePasser mp;

    std::vector<Extent<int>> boxes;
    int d = mp.Rank();
    for(int i = 0; i < 100; i++)
        boxes.push_back({{0,0,0},{d,d,d}});

    auto boundingBox = ParallelExtent::getBoundingBox(mp, boxes);

    int expectedHi = mp.NumberOfProcesses() - 1;
    REQUIRE(0.0 == Approx(boundingBox.lo[0]));
    REQUIRE(0.0 == Approx(boundingBox.lo[1]));
    REQUIRE(0.0 == Approx(boundingBox.lo[2]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[0]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[1]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[2]));
}
