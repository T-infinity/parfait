#include "CartBlockSlice.h"
#include "Point.h"

#include "catch.hpp"

using namespace Parfait;

TEST_CASE("Create a slice"){
    Point<double> p0 {0,0,0};
    Point<double> p1 {1,1,1};
    int n = 5;
    CartBlock block(p0.data(),p1.data(),n,n,n);

    CartBlockSlice slice(block,0,n*n*n);

    REQUIRE((slice.begin() != slice.end()));
    int count = 0;
    for(auto thing:slice){
        count++;
    }
    REQUIRE((n*n*n) == count);
}