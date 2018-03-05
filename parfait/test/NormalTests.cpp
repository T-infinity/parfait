
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
#include <parfait/Point.h>
#include <parfait/Normals.h>

TEST_CASE("Good Normal average"){
    Parfait::Point<double> n1 = {0,1,0};
    Parfait::Point<double> n2 = {1,0,0};

    auto n = Parfait::Normals::average({n1, n2});
    REQUIRE(n[0] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[1] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[2] == 0);
}

TEST_CASE("Good Normal with redundant normals"){
    Parfait::Point<double> n1 = {0,1,0};
    Parfait::Point<double> n2 = {1,0,0};

    auto n = Parfait::Normals::goodNormal({n1, n1, n1, n2});
    REQUIRE(n[0] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[1] == Approx(std::sqrt(2.0)/2.0));
    REQUIRE(n[2] == 0);
}

TEST_CASE("Good Normal, extreme case"){
    Parfait::Point<double> n1 = {1,0,0};
    Parfait::Point<double> n2 = {-1,0,0};
    Parfait::Point<double> n3 = {0,1,0};
    auto n = Parfait::Normals::goodNormal({n1, n2, n3});
    REQUIRE(n[0] == Approx(0));
    REQUIRE(n[1] == Approx(1));
    REQUIRE(n[2] == Approx(0));
}

