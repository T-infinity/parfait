#include "CartBlock.h"
#include "Point.h"

#include "catch.hpp"

using namespace Parfait;

TEST_CASE("Create a slice"){
    Point<double> p0 {0,0,0};
    Point<double> p1 {1,1,1};
    int n = 5;
    CartBlock block(p0.data(),p1.data(),n,n,n);


}