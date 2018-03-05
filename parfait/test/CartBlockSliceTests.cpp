
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
#include <parfait/CartBlock.h>
#include <parfait/Point.h>

#include "catch.hpp"

using namespace Parfait;

TEST_CASE("Create a slice"){
    Point<double> p0 {0,0,0};
    Point<double> p1 {1,1,1};
    int n = 5;
    CartBlock block(p0.data(),p1.data(),n,n,n);

    Extent<double> search_extent{p0,p1};

    auto slice = block.getRangeOfOverlappingCells(search_extent);
    REQUIRE(0 == slice.lo[0]);
    REQUIRE(0 == slice.lo[1]);
    REQUIRE(0 == slice.lo[2]);

    REQUIRE(5 == slice.hi[0]);
    REQUIRE(5 == slice.hi[1]);
    REQUIRE(5 == slice.hi[2]);

    int count = 0;
    for(int i=slice.lo[0];i<slice.hi[0];i++)
        for(int j=slice.lo[1];j<slice.hi[1];j++)
            for(int k=slice.lo[2];k<slice.hi[2];k++)
                count++;
    REQUIRE((n*n*n) == count);
}