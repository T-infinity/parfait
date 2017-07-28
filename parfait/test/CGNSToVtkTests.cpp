#include <catch.hpp>
#include <parfait/CellWindingConverters.h>

using namespace Parfait;


TEST_CASE("CGNSToVtk, tetsDontChange") {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = CGNSToVtk();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        REQUIRE(original[i] == tet[i]);
}
TEST_CASE("CGNSToVtk, pyramidsDontChange") {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = CGNSToVtk();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
        REQUIRE(pyramid[i] == original[i]);
}

TEST_CASE("CGNSToVtk, prismsDontChange") {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = CGNSToVtk();
    converter.convertPrism(prism.data());

    REQUIRE(original[0] == prism[3]);
    REQUIRE(original[1] == prism[4]);
    REQUIRE(original[2] == prism[5]);
    REQUIRE(original[3] == prism[0]);
    REQUIRE(original[4] == prism[1]);
    REQUIRE(original[5] == prism[2]);
}

TEST_CASE("CGNSToVtk, hexsDontChange") {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = CGNSToVtk();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        REQUIRE(original[i] == hex[i]);
}
