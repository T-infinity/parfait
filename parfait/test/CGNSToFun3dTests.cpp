
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include <catch.hpp>
#include <parfait/CellWindingConverters.h>

using namespace Parfait;

TEST_CASE("CGNSToFun3D, tetsDontChange") {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = CGNSToFun3D();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        REQUIRE(original[i] == tet[i]);
}

TEST_CASE("CGNSToFun3D, pyramidsDontChange") {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = CGNSToFun3D();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
        REQUIRE(pyramid[i] == original[i]);
}

TEST_CASE("CGNSToFun3D, prismsDoChange") {
    std::array<int,6> prism {9,11,13,17,21,6};
    auto converter = CGNSToFun3D();
    converter.convertPrism(prism.data());

    REQUIRE(prism[0] == 9);
    REQUIRE(prism[1] == 17);
    REQUIRE(prism[2] == 21);
    REQUIRE(prism[3] == 11);
    REQUIRE(prism[4] == 6);
    REQUIRE(prism[5] == 13);
}

TEST_CASE("CGNSToFun3D, hexsDoChange") {
    std::array<int,8> hex {9,6,5,12,3,1,14,7};
    auto converter = CGNSToFun3D();
    converter.convertHex(hex.data());

    REQUIRE(hex[0] == 9);
    REQUIRE(hex[1] == 3);
    REQUIRE(hex[2] == 6);
    REQUIRE(hex[3] == 1);
    REQUIRE(hex[4] == 12);
    REQUIRE(hex[5] == 7);
    REQUIRE(hex[6] == 5);
    REQUIRE(hex[7] == 14);
}
