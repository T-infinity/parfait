
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


TEST_CASE("SumIntegers"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0;
    int value = mp->Rank();
    int psum = 0;
    psum = mp->ParallelSum(value,root);
    if(mp->Rank() == root)
    {
        int sum = 0;
        for(int i=0;i<mp->NumberOfProcesses();i++)
            sum += i;
        REQUIRE(sum == psum);
    }
}

TEST_CASE("SumFloats"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0;
    float junk = 1.7e-1;
    float value = junk + (float)mp->Rank();
    float psum = 0.0;
    psum = mp->ParallelSum(value,root);
    if(mp->Rank() == root)
    {
        float sum = 0.0;
        for(int i=0;i< mp->NumberOfProcesses();i++)
            sum += junk + (float)i;
        REQUIRE(sum == Approx(psum));
    }
}

TEST_CASE("SumDoubles"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    double junk = 1.7e-9;
    double value = junk + (double)mp->Rank();
    double psum = 0.0;
    psum = mp->ParallelSum(value,root);
    if(mp->Rank() == root) {
        double sum = 0.0;
        for(int i=0;i<mp->NumberOfProcesses();i++)
            sum += junk + (double)i;
        REQUIRE(sum == Approx(psum));
    }
}
