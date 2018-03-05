
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
#include <iostream>
#include "MinimumStorageSet.h"

TEST_CASE("insert things uniquely"){
    Parfait::MinimumStorageSet<int> minimumStorageSet;

    minimumStorageSet.insert(0);
    REQUIRE(1 == minimumStorageSet.size());

    minimumStorageSet.insert(0);
    REQUIRE(1 == minimumStorageSet.size());

    minimumStorageSet.insert(9);
    minimumStorageSet.insert(8);

    REQUIRE(3 == minimumStorageSet.size());

    std::vector<int> vec = minimumStorageSet.convertToVector();
    REQUIRE(3 == vec.size());
    REQUIRE(0 == vec[0]);
    REQUIRE(8 == vec[1]);
    REQUIRE(9 == vec[2]);
}