
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
#include <MessagePasser/MessagePasser.h>
#include <parfait/Extent.h>
#include <parfait/ParallelExtent.h>
#include <catch.hpp>

using namespace Parfait;


TEST_CASE("SingleBox"){
    MessagePasser mp;
    Extent<double> e({0,0,0},{1,1,1});
    e.hi[0] = mp.Rank();
    e.hi[1] = mp.Rank();
    e.hi[2] = mp.Rank();

    auto boundingBox = ParallelExtent::getBoundingBox(mp,e);
    REQUIRE(0.0 == Approx(boundingBox.lo[0]));
    REQUIRE(0.0 == Approx(boundingBox.lo[1]));
    REQUIRE(0.0 == Approx(boundingBox.lo[2]));

    int expectedHi = mp.NumberOfProcesses() - 1;

    REQUIRE(expectedHi == Approx(boundingBox.hi[0]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[1]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[2]));
}

TEST_CASE("VectorOfBoxes"){
    MessagePasser mp;

    std::vector<Extent<int>> boxes;
    int d = mp.Rank();
    for(int i = 0; i < 100; i++)
        boxes.push_back({{0,0,0},{d,d,d}});

    auto boundingBox = ParallelExtent::getBoundingBox(mp, boxes);

    int expectedHi = mp.NumberOfProcesses() - 1;
    REQUIRE(0.0 == Approx(boundingBox.lo[0]));
    REQUIRE(0.0 == Approx(boundingBox.lo[1]));
    REQUIRE(0.0 == Approx(boundingBox.lo[2]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[0]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[1]));
    REQUIRE(expectedHi == Approx(boundingBox.hi[2]));
}
