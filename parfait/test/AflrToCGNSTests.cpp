
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

TEST_CASE("AflrToCGNS, tetsDontChange") {
    std::array<int,4> tet {0,1,2,3};
    auto original = tet;
    auto converter = AflrToCGNS();
    converter.convertTet(tet.data());

    for(int i=0;i<4;i++)
        REQUIRE(original[i]== tet[i]);
}

TEST_CASE("AflrToCGNS, pyramidsDoChange") {
    std::array<int,5> pyramid {9,12,4,2,16};
    auto original = pyramid;
    auto converter = AflrToCGNS();
    converter.convertPyramid(pyramid.data());

    REQUIRE(pyramid[0]==original[0]);
    REQUIRE(pyramid[1]==original[3]);
    REQUIRE(pyramid[2]==original[4]);
    REQUIRE(pyramid[3]==original[1]);
    REQUIRE(pyramid[4]==original[2]);
}

TEST_CASE("AflrToCGNS, prismsDontChange") {
    std::array<int,6> prism {0,1,2,3,4,5};
    auto original = prism;
    auto converter = AflrToCGNS();
    converter.convertPrism(prism.data());

    for(int i=0;i<6;i++)
        REQUIRE(original[i]== prism[i]);
}

TEST_CASE("AflrToCGNS, hexsDontChange") {
    std::array<int,8> hex {0,1,2,3,4,5,6,7};
    auto original = hex;
    auto converter = AflrToCGNS();
    converter.convertHex(hex.data());

    for(int i=0;i<8;i++)
        REQUIRE(original[i] == hex[i]);
}
