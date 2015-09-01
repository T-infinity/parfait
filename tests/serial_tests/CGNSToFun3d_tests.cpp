#include "CellWindingConverters.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace Parfait;


TEST_GROUP(CGNSToFun3D){

};
TEST(CGNSToFun3D, tetsDontChange) {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = CGNSToFun3D();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        LONGS_EQUAL(original[i], tet[i]);
}

TEST(CGNSToFun3D, pyramidsDontChange) {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = CGNSToFun3D();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
        LONGS_EQUAL(pyramid[i], original[i]);
}

TEST(CGNSToFun3D, prismsDoChange) {
    std::array<int,6> prism {9,11,13,17,21,6};
    auto original = prism;
    auto converter = CGNSToFun3D();
    converter.convertPrism(prism.data());

    LONGS_EQUAL(prism[0],9);
    LONGS_EQUAL(prism[1],17);
    LONGS_EQUAL(prism[2],21);
    LONGS_EQUAL(prism[3],11);
    LONGS_EQUAL(prism[4],6);
    LONGS_EQUAL(prism[5],13);
}

TEST(CGNSToFun3D, hexsDoChange) {
    std::array<int,8> hex {9,6,5,12,3,1,14,7};
    auto original = hex;
    auto converter = CGNSToFun3D();
    converter.convertHex(hex.data());

    LONGS_EQUAL(hex[0],9);
    LONGS_EQUAL(hex[1],3);
    LONGS_EQUAL(hex[2],6);
    LONGS_EQUAL(hex[3],1);
    LONGS_EQUAL(hex[4],12);
    LONGS_EQUAL(hex[5],7);
    LONGS_EQUAL(hex[6],5);
    LONGS_EQUAL(hex[7],14);
}
