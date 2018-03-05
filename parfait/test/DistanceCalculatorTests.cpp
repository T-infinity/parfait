
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
#include "catch.hpp"
#include <parfait/DistanceCalculator.h>
using namespace Parfait;


TEST_CASE("get distance to triangle"){
    Parfait::Facet f({0,0,0}, {.5,0,0}, {0,.5,0});

    auto fillPoint = [&] (int id,double* p){for(int i=0;i<3;i++) p[i] = f[id][i];};
    auto fillCell  = [] (int id,int* face){for(int i=0;i<3;i++) face[i] = i;};
    auto getCellType = [] (int id) {return DistanceCalculator::TRI_3;};

    MessagePasser mp;

    DistanceCalculator distanceCalculator(mp,fillPoint,fillCell,getCellType,1);

    REQUIRE(0 == Approx(distanceCalculator.distance({0,0,0})));
}

TEST_CASE("query distance with no geometry"){
    Parfait::Facet f({0,0,0}, {.5,0,0}, {0,.5,0});

    auto fillPoint = [&] (int id,double* p){for(int i=0;i<3;i++) p[i] = f[id][i];};
    auto fillCell  = [] (int id,int* face){for(int i=0;i<3;i++) face[i] = i;};
    auto getCellType = [] (int id) {return DistanceCalculator::TRI_3;};

    MessagePasser mp;

    int facet_count = 0;

    DistanceCalculator dc(mp, fillPoint, fillCell, getCellType, facet_count);

    auto d = dc.distance({0,0,0});
}