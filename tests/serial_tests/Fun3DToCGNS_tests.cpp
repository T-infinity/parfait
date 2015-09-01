#include "CellWindingConverters.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;

TEST_GROUP(Fun3DToCGNS){

};
TEST(Fun3DToCGNS, tetsDontChange) {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = Fun3DToCGNS();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
    LONGS_EQUAL(original[i], tet[i]);
}

TEST(Fun3DToCGNS, pyramidsDontChange) {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = Fun3DToCGNS();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
    LONGS_EQUAL(pyramid[i], original[i]);
}

TEST(Fun3DToCGNS, prismsDoChange) {
    std::array<int,6> prism {9,17,21,11,6,13};
    auto original = prism;
    auto converter = Fun3DToCGNS();
    converter.convertPrism(prism.data());

    LONGS_EQUAL(prism[0],9);
    LONGS_EQUAL(prism[1],11);
    LONGS_EQUAL(prism[2],13);
    LONGS_EQUAL(prism[3],17);
    LONGS_EQUAL(prism[4],21);
    LONGS_EQUAL(prism[5],6);
}

TEST(Fun3DToCGNS, hexsDoChange) {
    std::array<int,8> hex {9,3,6,1,12,7,5,14};
    auto original = hex;
    auto converter = Fun3DToCGNS();
    converter.convertHex(hex.data());

    LONGS_EQUAL(hex[0],9);
    LONGS_EQUAL(hex[1],6);
    LONGS_EQUAL(hex[2],5);
    LONGS_EQUAL(hex[3],12);
    LONGS_EQUAL(hex[4],3);
    LONGS_EQUAL(hex[5],1);
    LONGS_EQUAL(hex[6],14);
    LONGS_EQUAL(hex[7],7);
}

