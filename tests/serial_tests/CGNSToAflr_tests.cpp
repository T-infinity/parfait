#include "CellWindingConverters.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;


TEST_GROUP(CGNSToAflr){

};

TEST(CGNSToAflr, tetsDontChange) {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = CGNSToAflr();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        LONGS_EQUAL(original[i], tet[i]);
}

TEST(CGNSToAflr, pyramidsDoChange) {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = CGNSToAflr();
    converter.convertPyramid(pyramid.data());

    LONGS_EQUAL(pyramid[0],original[0]);
    LONGS_EQUAL(pyramid[1],original[1]);
    LONGS_EQUAL(pyramid[2],original[4]);
    LONGS_EQUAL(pyramid[3],original[2]);
    LONGS_EQUAL(pyramid[4],original[3]);
}

TEST(CGNSToAflr, prismsDontChange) {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = CGNSToAflr();
    converter.convertPrism(prism.data());

    for(int i=0;i<6;i++)
        LONGS_EQUAL(original[i], prism[i]);
}

TEST(CGNSToAflr, hexsDontChange) {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = CGNSToAflr();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        LONGS_EQUAL(original[i], hex[i]);
}
