#include "CartBlock.h"
#include "Point.h"

#include "catch.hpp"

using namespace Parfait;

TEST_CASE("Create a slice"){
    Point<double> p0 {0,0,0};
    Point<double> p1 {1,1,1};
    int n = 5;
    CartBlock block(p0.data(),p1.data(),n,n,n);

    Extent<double> search_extent{p0,p1};

    auto slice = block.getRangeOfOverlappingCells(search_extent);
    REQUIRE(0 == slice.lo[0]);
    REQUIRE(0 == slice.lo[1]);
    REQUIRE(0 == slice.lo[2]);

    REQUIRE(5 == slice.hi[0]);
    REQUIRE(5 == slice.hi[1]);
    REQUIRE(5 == slice.hi[2]);

    int count = 0;
    for(int i=slice.lo[0];i<slice.hi[0];i++)
        for(int j=slice.lo[1];j<slice.hi[1];j++)
            for(int k=slice.lo[2];k<slice.hi[2];k++)
                count++;
    REQUIRE((n*n*n) == count);
}