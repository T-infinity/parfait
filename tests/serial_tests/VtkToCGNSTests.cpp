#include <catch.hpp>
#include "CellWindingConverters.h"
using namespace Parfait;

TEST_CASE("VtkToCGNS, tetsDontChang") {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = VtkToCGNS();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        REQUIRE(original[i] == tet[i]);
}
TEST_CASE("VtkToCGNS, pyramidsDontChange") {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = VtkToCGNS();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
        REQUIRE(pyramid[i] == original[i]);
}

TEST_CASE("VtkToCGNS, prismsDontChange") {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = VtkToCGNS();
    converter.convertPrism(prism.data());

    REQUIRE(original[0] == prism[3]);
    REQUIRE(original[1] == prism[4]);
    REQUIRE(original[2] == prism[5]);
    REQUIRE(original[3] == prism[0]);
    REQUIRE(original[4] == prism[1]);
    REQUIRE(original[5] == prism[2]);
}

TEST_CASE("VtkToCGNS, hexsDontChange") {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = VtkToCGNS();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        REQUIRE(original[i] == hex[i]);
}
