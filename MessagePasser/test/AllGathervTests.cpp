
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
#include <vector>
#include "../MessagePasser.h"
#include <catch.hpp>

TEST_CASE("TestWithIntegers"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    std::vector<int> map;
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(mp->Rank()+i);
    mp->AllGatherv(send_vec,recv_vec,map);
    REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
    REQUIRE(map.back() == (int)recv_vec.size());
    for(int i=0;i< mp->NumberOfProcesses();i++){
        int counter = 0;
        for(int j=map[i];j<map[i+1];j++){
            REQUIRE((i+counter) == recv_vec[j]);
            ++counter;
        }
    }
}

TEST_CASE("TestWithNoMapPassedFromRoot"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    std::vector<int> map(mp->NumberOfProcesses()+1);
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(mp->Rank()+i);
    mp->AllGatherv(send_vec,recv_vec);

    for(int i=1;i<mp->NumberOfProcesses()+1;i++){
        num = (i-1)%2==0 ? 2 : 3;
        map[i] = map[i-1] + num;
    }
    REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
    REQUIRE(map.back() == (int)recv_vec.size());
    for(int i=0;i<mp->NumberOfProcesses();i++) {
        int counter = 0;
        for(int j=map[i];j<map[i+1];j++) {
            REQUIRE((i+counter) == recv_vec[j]);
            ++counter;
        }
    }
}

TEST_CASE("TestWithVectorOfVectors"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector <int> send_vec;
    std::vector <std::vector<int>> result;
    std::vector <int> map(mp->NumberOfProcesses() + 1);
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if (mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for (int i = 0; i < num; i++)
        send_vec.push_back(mp->Rank() + i);
    mp->AllGatherv(send_vec, result);
    for(int i=0;i<mp->NumberOfProcesses();i++){
        if(i % 2 == 0) {
            REQUIRE(2 == result[i].size());
            for(int j=0;j<2;j++)
                REQUIRE((i+j) == result[i][j]);
        }
        else{
            REQUIRE(3 == result[i].size());
            for(int j=0;j<3;j++)
                REQUIRE((i+j) == result[i][j]);
        }

    }
}
