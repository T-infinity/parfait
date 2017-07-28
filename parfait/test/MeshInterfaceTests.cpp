#include <catch.hpp>
#include <parfait/VolumeMeshInterface.h>
#include <parfait/CartBlock.h>
#include <parfait/VolumeMeshAdapter.h>

using namespace Parfait;

TEST_CASE("VolumeMeshInterface Exists"){
    CartBlock cartBlock({0,0,0},{1,1,1}, 5,5,8);
    VolumeMeshAdapter adapter(cartBlock);

    REQUIRE(cartBlock.numberOfNodes() == adapter.numberOfNodes());
}


