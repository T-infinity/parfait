#include <catch.hpp>
#include "CellWindingConverters.h"

using namespace Parfait;

TEST_CASE("CGNSToAflr, tetsDontChange") {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = CGNSToAflr();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        REQUIRE(original[i] == tet[i]);
}

TEST_CASE("CGNSToAflr, pyramidsDoChange") {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = CGNSToAflr();
    converter.convertPyramid(pyramid.data());

    REQUIRE(pyramid[0] ==original[0]);
    REQUIRE(pyramid[1] ==original[1]);
    REQUIRE(pyramid[2] ==original[4]);
    REQUIRE(pyramid[3] ==original[2]);
    REQUIRE(pyramid[4] ==original[3]);
}

TEST_CASE("CGNSToAflr, prismsDontChange") {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = CGNSToAflr();
    converter.convertPrism(prism.data());

    for(int i=0;i<6;i++)
        REQUIRE(original[i] == prism[i]);
}

TEST_CASE("CGNSToAflr, hexsDontChange") {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = CGNSToAflr();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        REQUIRE(original[i] == hex[i]);
}
