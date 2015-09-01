#include "CellWindingConverters.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;


TEST_GROUP(VtkToCGNS){

};

TEST(VtkToCGNS, tetsDontChange) {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = VtkToCGNS();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        LONGS_EQUAL(original[i], tet[i]);
}
TEST(VtkToCGNS, pyramidsDontChange) {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = VtkToCGNS();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
        LONGS_EQUAL(pyramid[i],original[i]);
}

TEST(VtkToCGNS, prismsDontChange) {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = VtkToCGNS();
    converter.convertPrism(prism.data());

    LONGS_EQUAL(original[0], prism[3]);
    LONGS_EQUAL(original[1], prism[4]);
    LONGS_EQUAL(original[2], prism[5]);
    LONGS_EQUAL(original[3], prism[0]);
    LONGS_EQUAL(original[4], prism[1]);
    LONGS_EQUAL(original[5], prism[2]);
}

TEST(VtkToCGNS, hexsDontChange) {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = VtkToCGNS();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        LONGS_EQUAL(original[i], hex[i]);
}
