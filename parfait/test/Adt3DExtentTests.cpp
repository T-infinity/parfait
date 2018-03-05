
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
#include <parfait/Adt3dExtent.h>

using namespace Parfait;

TEST_CASE("Adt3DExtent, StoreExtent") {
    Adt3DExtent adt(Extent<double>(Point<double>(0,0,0), Point<double>(1,1,1)));
    Extent<double> store1(Point<double>(.1,.1,.1), Point<double>(.2,.2,.2));
    Extent<double> store2(Point<double>(.7,.7,.7), Point<double>(.8,.8,.8));

    adt.store(10, store1);
    adt.store(11, store2);

    std::vector<int> inside;
    Extent<double> extent(Point<double>(0,0,0), Point<double>(1,1,1));
    inside = adt.retrieve(extent);

    REQUIRE(2 == inside.size());

    extent = Extent<double>(Point<double>(0,0,0), Point<double>(.5,.5,.5));
    inside = adt.retrieve(extent);
    REQUIRE(1 == inside.size());
    REQUIRE(10 == inside[0]);
}

TEST_CASE("Adt3DExtent, StoreNegativeExtent") {

    Adt3DExtent adt(Extent<double>(Point<double>(-2,-2,-2), Point<double>(2,2,2)));
    Extent<double> store1(Point<double>(.1,.1,.1), Point<double>(.2,.2,.2));
    Extent<double> store2(Point<double>(.7,.7,.7), Point<double>(.8,.8,.8));

    adt.store(10, store1);
    adt.store(11, store2);

    std::vector<int> inside;
    Extent<double> extent(Point<double>(0,0,0), Point<double>(1,1,1));
    inside = adt.retrieve(extent);

    REQUIRE(2 == inside.size());

    extent = Extent<double>(Point<double>(0,0,0), Point<double>(.5,.5,.5));
    inside = adt.retrieve(extent);
    REQUIRE(1 == inside.size());
    REQUIRE(10 == inside[0]);

    adt.store(13, Extent<double>(Point<double>(-.5,-1,-1), Point<double>(0,0,0)));
    inside = adt.retrieve(Extent<double>(Point<double>(-4,-4,-4), Point<double>(4,4,4)));
    REQUIRE(3 == inside.size());
}


TEST_CASE("Adt3DExtent, With two elements") {
    Adt3DExtent adt({{0,0,0}, {1,1,1}});

    adt.store(6, {{0,0,0},{.1,.1,.1}});
    adt.store(7, {{.5,.5,.5}, {1,1,1}});
    adt.removeFirst(6, {{0,0,0},{1,1,1}});

    auto inside = adt.retrieve({{0,0,0},{.1,.1,.1}});
    REQUIRE(inside.size() == 0);

    inside = adt.retrieve({{.5,.5,.5},{1,1,1}});
    REQUIRE(inside.size() == 1);
    REQUIRE(7 == inside[0]);
}

TEST_CASE("Adt3DExtent, Delete Leaving Zero Elements") {
    Adt3DExtent adt({{0,0,0}, {1,1,1}});

    adt.store(6, {{0,0,0},{.1,.1,.1}});
    adt.removeFirst(6, {{0,0,0},{1,1,1}});

    auto inside = adt.retrieve({{0,0,0},{.1,.1,.1}});
    REQUIRE(inside.size() == 0);

}

TEST_CASE("Adt3DExtent, Delete From Empty Tree") {
    Adt3DExtent adt({{0,0,0}, {1,1,1}});

    adt.removeFirst(6, {{0,0,0},{1,1,1}});

    auto inside = adt.retrieve({{0,0,0},{.1,.1,.1}});
    REQUIRE(inside.size() == 0);

}