#include "CellWindingConverters.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;


TEST_GROUP(AflrToCGNS){

};

TEST(AflrToCGNS, tetsDontChange) {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = AflrToCGNS();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        LONGS_EQUAL(original[i], tet[i]);
}

TEST(AflrToCGNS, pyramidsDoChange) {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = AflrToCGNS();
    converter.convertPyramid(pyramid.data());

    LONGS_EQUAL(pyramid[0],original[0]);
    LONGS_EQUAL(pyramid[1],original[1]);
    LONGS_EQUAL(pyramid[2],original[3]);
    LONGS_EQUAL(pyramid[3],original[4]);
    LONGS_EQUAL(pyramid[4],original[2]);
}

TEST(AflrToCGNS, prismsDontChange) {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = AflrToCGNS();
    converter.convertPrism(prism.data());

    for(int i=0;i<6;i++)
        LONGS_EQUAL(original[i], prism[i]);
}

TEST(AflrToCGNS, hexsDontChange) {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = AflrToCGNS();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        LONGS_EQUAL(original[i], hex[i]);
}
