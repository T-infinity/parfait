
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


TEST_CASE("Parallel max of each element in a vector"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec(mp->NumberOfProcesses(),mp->Rank());
    std::vector<int> result = mp->ParallelMax(vec,root);
    if(mp->Rank() == root)
        for(int i=0;i<mp->NumberOfProcesses();i++)
            REQUIRE((mp->NumberOfProcesses()-1) == result[i]);
}

TEST_CASE("Parallel min of each element in a vector"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec(mp->NumberOfProcesses(),mp->Rank()+7);
    std::vector<int> result = mp->ParallelMin(vec,root);
    if(mp->Rank() == root)
        for(int i=0;i<mp->NumberOfProcesses();i++)
            REQUIRE(7 == result[i]);
}

TEST_CASE("Parallel max reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    int value = mp->Rank();
    value = mp->ParallelMax(value, root);
    if(mp->Rank() == root)
        REQUIRE((mp->NumberOfProcesses()-1 == value));
    else
        REQUIRE(value == mp->Rank());
}

TEST_CASE("Parallel min reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = mp->NumberOfProcesses()-1;
    int value = mp->Rank();
    value = mp->ParallelMin(value, root);
    if(mp->Rank() == root)
        REQUIRE(0 == value);
    else
        REQUIRE(value == mp->Rank());
}

TEST_CASE("Parallel rank of max reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int value = mp->Rank();
    int rank = mp->ParallelRankOfMax(value);
    REQUIRE((mp->NumberOfProcesses()-1 == rank));
}

TEST_CASE("Parallel min-all reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = mp->NumberOfProcesses()-1;
    int value = mp->Rank();
    value = mp->ParallelMin(value);
    REQUIRE(0 == value);
}

TEST_CASE("Parallel max-all reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int value = mp->Rank();
    value = mp->ParallelMax(value);
    REQUIRE((mp->NumberOfProcesses()-1 == value));
}

TEST_CASE("Parallel sum of an integer"){
    auto mp = std::make_shared<MessagePasser>();

    auto sum = mp->ParallelSum(1);
    REQUIRE(mp->NumberOfProcesses() == sum);
}

TEST_CASE("Elemental max"){
    auto mp = std::make_shared<MessagePasser>();

    std::vector<int> vec(mp->NumberOfProcesses(),0);
    vec[mp->Rank()] = mp->Rank();
    auto result = mp->AllElementalMax(vec);
    REQUIRE(mp->NumberOfProcesses() == result.size());
    for(int i=0;i<mp->NumberOfProcesses();++i){
        REQUIRE(i == result[i]);
    }
}
