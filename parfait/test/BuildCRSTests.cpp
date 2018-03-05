
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
#include <parfait/CRS.h>

TEST_CASE("Build CRS"){
    std::vector<std::vector<int>> graph = {
        {0,1,2}, 
        {0,1,2,3,4},
        {0,1,2,3,6},
        {1,2,3,4,6},
        {1,3,4,5},
        {4,5,6},
        {2,3,5,6}
    };

    Parfait::CRS crs = Parfait::CRS::buildCRSFromGraph(graph);

    REQUIRE(8 == crs.ia.size());
    REQUIRE(0 == crs.ia[0]);
    REQUIRE(3 == crs.ia[1]);
    REQUIRE(6 == crs.ja[12]);
    REQUIRE(20 == crs.iau[4]);
}
