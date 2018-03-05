
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
#include "catch.hpp"


#define MPI_FLOAT_TOL 1.0e-6
#define MPI_DOUBLE_TOL 1.0e-15


TEST_CASE("Test Gatherv for integers"){
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
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(map.back() == (int)recv_vec.size());
        for(int i = 0; i < mp->NumberOfProcesses(); i++) {
            int counter = 0;
            for(int j = map[i]; j < map[i+1]; j++) {
                REQUIRE((i+counter) == recv_vec[j]);
                ++counter;
            }
        }
    }

}

TEST_CASE("TestWithIntegersNoMapPassedIn"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(mp->Rank()+i);
    mp->Gatherv(send_vec,recv_vec,root);
    // just make sure it gets called, since it is just a delegation
}

TEST_CASE("TestWithFloats"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<float> send_vec;
    std::vector<float> recv_vec;
    float junk = 1.7e-5;
    std::vector<int> map;
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(junk+(float)(mp->Rank()+i));
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(map.back() == (int)recv_vec.size());
        for(int i=0;i<mp->NumberOfProcesses();i++) {
            int counter = 0;
            for(int j=map[i];j<map[i+1];j++) {
                REQUIRE((junk+(float)(i+counter)) == Approx(recv_vec[j]));
                ++counter;
            }
        }
    }
}

TEST_CASE("TestWithDoubles"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<double> send_vec;
    std::vector<double> recv_vec;
    double junk = 1.7e-13;
    std::vector<int> map;
    int num = 0;
// even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(junk+(double)(mp->Rank()+i));
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(map.back() == (int)recv_vec.size());
        for(int i=0;i<mp->NumberOfProcesses();i++) {
            int counter = 0;
            for(int j=map[i];j<map[i+1];j++) {
                REQUIRE((junk+(double)(i+counter)) == Approx(recv_vec[j]));
                ++counter;
            }
        }
    }
}

TEST_CASE("TestWhenAllVectorsAreEmpty"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    std::vector<int> map;
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(0 == (int)recv_vec.size());
        REQUIRE(0 == map.back());
    }
}

TEST_CASE("TestGatheringAsVectorOfVectors"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<double> send_vec;
    std::vector<std::vector<double>> result;
    double junk = 1.7e-13;
    std::vector<int> map;
    int num = 0;
// even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(junk+(double)(mp->Rank()+i));
    mp->Gatherv(send_vec, result,root);
    if(mp->Rank() == root) {
        REQUIRE(mp->NumberOfProcesses() == (int) result.size());
        for(int i=0;i<mp->NumberOfProcesses();i++) {
            int counter = 0;
            REQUIRE((i%2==0?2:3) == result[i].size());
            for(unsigned int j=0;j< result[i].size();j++) {
                REQUIRE((junk+(double)(i+counter)) == Approx(result[i][j]));
                ++counter;
            }
        }
    }
}
