#include "catch.hpp"
#include <parfait/AdtDistanceCalculator.h>
using namespace Parfait;
TEST_CASE("get distance to triangle"){
    Parfait::Facet f({0,0,0}, {.5,0,0}, {0,.5,0});

    auto fillPoint = [&] (int id,double* p){for(int i=0;i<3;i++) p[i] = f[id][i];};
    auto fillCell  = [] (int id,int* face){for(int i=0;i<3;i++) face[i] = i;};
    auto getCellType = [] (int id) {return DistanceCalculator::TRI_3;};

    auto mp = std::make_shared<MessagePasser>();

    DistanceCalculator distanceCalculator(mp,fillPoint,fillCell,getCellType,1);

    REQUIRE(0 == Approx(distanceCalculator.distance({0,0,0})));
}