#include <catch.hpp>
#include <CompressedMapExtras.h>

TEST_CASE("Compressed Map"){
    using namespace Parfait;
    REQUIRE(getIndexOfItemInRange({0,1,2,3}, 0) == 0);
    REQUIRE(getIndexOfItemInRange({0,1,2,3}, 2) == 2);

    REQUIRE(getIndexOfItemInRange({0,1,2,30}, 29) == 2);
    REQUIRE_NOTHROW(getIndexOfItemInRange({0,1,2,30}, 30));
    REQUIRE_THROWS(getIndexOfItemInRange({0,1,2,30}, -1));
}