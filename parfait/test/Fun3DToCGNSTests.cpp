
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

TEST_CASE("Fun3DToCGNS, tetsDontChange") {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = Fun3DToCGNS();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
    REQUIRE(original[i] == tet[i]);
}

TEST_CASE("Fun3DToCGNS, pyramidsDontChange") {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = Fun3DToCGNS();
    converter.convertPyramid(pyramid.data());

    for(int i=0;i<5;i++)
    REQUIRE(pyramid[i] == original[i]);
}

TEST_CASE("Fun3DToCGNS, prismsDoChange") {
    std::array<int,6> prism {9,17,21,11,6,13};
    auto converter = Fun3DToCGNS();
    converter.convertPrism(prism.data());

    REQUIRE(prism[0] == 9);
    REQUIRE(prism[1] == 11);
    REQUIRE(prism[2] == 13);
    REQUIRE(prism[3] == 17);
    REQUIRE(prism[4] == 21);
    REQUIRE(prism[5] == 6);
}

TEST_CASE("Fun3DToCGNS, hexsDoChange") {
    std::array<int,8> hex {9,3,6,1,12,7,5,14};
    auto converter = Fun3DToCGNS();
    converter.convertHex(hex.data());

    REQUIRE(hex[0] == 9);
    REQUIRE(hex[1] == 6);
    REQUIRE(hex[2] == 5);
    REQUIRE(hex[3] == 12);
    REQUIRE(hex[4] == 3);
    REQUIRE(hex[5] == 1);
    REQUIRE(hex[6] == 14);
    REQUIRE(hex[7] == 7);
}

