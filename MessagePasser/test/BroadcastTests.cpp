
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


TEST_CASE("Broadcast an integer"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,val=0;
    if(mp->Rank() == root)
        val = 5;
    mp->Broadcast(val,root);
    REQUIRE(5 == val);
}

TEST_CASE("Broadcast a double"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    double val=0,check_val=5.0 + 1.7e-13;
    if(mp->Rank() == root)
        val = check_val;
    mp->Broadcast(val,root);
    REQUIRE(check_val == Approx(val));
}

TEST_CASE("Broadcast vector of ints"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,val=0;
    std::vector<int> vec(5,0);
    if(mp->Rank() == root)
        for(int i=0;i<5;i++)
            vec[i] = i;
    mp->Broadcast(vec,5,root);
    REQUIRE(5 == vec.size());
    for(int i=0;i<5;i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("Broadcast string"){
    std::string s = "dog";
    auto mp = MessagePasser();
    mp.Broadcast(s,0);
    REQUIRE(3 == s.size());
}

TEST_CASE("Broadcast a vector of unspecified size"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,val=0;
    std::vector<int> vec;
    if(mp->Rank() == root)
        for(int i=0;i<5;i++)
            vec.push_back(i);
    mp->Broadcast(vec,root);
    REQUIRE(5 == vec.size());
    for(int i=0;i<5;i++)
        REQUIRE(i == vec[i]);
}