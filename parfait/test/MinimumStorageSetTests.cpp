#include <catch.hpp>
#include <iostream>
#include "MinimumStorageSet.h"

TEST_CASE("insert things uniquely"){
    Parfait::MinimumStorageSet<int> minimumStorageSet;

    minimumStorageSet.insert(0);
    REQUIRE(1 == minimumStorageSet.size());

    minimumStorageSet.insert(0);
    REQUIRE(1 == minimumStorageSet.size());

    minimumStorageSet.insert(9);
    minimumStorageSet.insert(8);

    REQUIRE(3 == minimumStorageSet.size());

    std::vector<int> vec = minimumStorageSet.convertToVector();
    REQUIRE(3 == vec.size());
    REQUIRE(0 == vec[0]);
    REQUIRE(8 == vec[1]);
    REQUIRE(9 == vec[2]);
}