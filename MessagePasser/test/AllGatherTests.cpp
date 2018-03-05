
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
#include "../MessagePasser.h"
#include "catch.hpp"

TEST_CASE("integer scalars"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0,value=mp->Rank();
    std::vector<int> vec;
    mp->AllGather(value,vec);
    int nproc = mp->NumberOfProcesses();
    REQUIRE(nproc == (int)vec.size());
    for(int i=0;i<nproc;i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("float scalars"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    double junk = 1.7e-13;
    double value=junk + (double)mp->Rank();
    std::vector<double> vec;
    mp->AllGather(value,vec);
    int nproc = mp->NumberOfProcesses();
    REQUIRE(nproc == (int)vec.size());
    for(int i=0;i<nproc;i++)
        REQUIRE((junk+(double)i) == vec[i]);
}


