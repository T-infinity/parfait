
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

#include <parfait/STL.h>
#include <parfait/STLFactory.h>

using namespace Parfait;


TEST_CASE("STLTests, Closest"){
    auto stl = STLFactory::getUnitCube();
    stl.scaleToUnitLength();

    Parfait::STL::SearchSTL searchSTL(stl);
    double distance;
    auto p = searchSTL.getClosestPoint({2, 2, 2});
    REQUIRE(p.approxEqual({.5,.5,.5}));
}

TEST_CASE("STLTests, ClosestBugFixCheck"){
    auto stl = STLFactory::getUnitCube();
    stl.scaleToUnitLength();

    Parfait::STL::SearchSTL searchSTL(stl);
    Parfait::Point<double> p(-1,1,1);
    auto closest = searchSTL.getClosestPoint(p);
    REQUIRE(closest.approxEqual({-.5,.5,.5}));
}

TEST_CASE("STLTests, findDomain"){
    auto stl = STLFactory::getUnitCube();
    stl.scaleToUnitLength();
    auto domain = stl.findDomain();
    REQUIRE(-0.5 == Approx(domain.lo[0]));
    REQUIRE( 0.5 == Approx(domain.hi[0]));
}

TEST_CASE("STL add facet"){
    std::array<double, 3> p1 = {0,0,0};
    std::array<double, 3> p2 = {1,0,0};
    std::array<double, 3> p3 = {1,1,0};

    Parfait::STL::STL stl;
    stl.addFacet(p1, p2, p3);
    REQUIRE(stl.facets[0].points[0][0] == Approx(0));
    REQUIRE(stl.facets[0].points[2][0] == Approx(1));
}

TEST_CASE("STL scaled"){
    std::array<double, 3> p1 = {0,0,0};
    std::array<double, 3> p2 = {1,0,0};
    std::array<double, 3> p3 = {1,1,0};

    Parfait::STL::STL stl;
    stl.addFacet(p1, p2, p3);
    stl.rescale(10);
    REQUIRE(stl.facets[0].points[0][0] == Approx(0.0));
    REQUIRE(stl.facets[0].points[2][0] == Approx(10.0));
}

TEST_CASE("Ensure we project to facets with large extent boxes"){
    Parfait::STL::STL stl;
    std::array<double, 3> p1 = {0,0,0};
    std::array<double, 3> p2 = {1,0,0};
    std::array<double, 3> p3 = {1,1,1};
    stl.addFacet(p1, p2, p3);
    p1 = {0,0,.8};
    p2 = {.9,0,.8};
    p3 = {.9,.1,.8};
    stl.addFacet(p1, p2, p3);
    Parfait::STL::SearchSTL searchSTL(stl);

    auto p = searchSTL.getClosestPoint({1,0,1});
    REQUIRE(p[0] == Approx(0.9));
    REQUIRE(p[1] == Approx(0.0));
    REQUIRE(p[2] == Approx(0.8));
}
