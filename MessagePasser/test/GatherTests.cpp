
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


TEST_CASE("Gather for integers"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,value=mp->Rank();
    std::vector<int> vec;
    mp->Gather(value,vec,root);
    // check that they were communicated properly to the root
    if(mp->Rank() == root) {
        int nproc = mp->NumberOfProcesses();
        REQUIRE(nproc == (int)vec.size());
        for(int i=0;i<nproc;i++)
            REQUIRE(i == vec[i]);
    }
}

TEST_CASE("Gather floats"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    float junk = 1.7e-12;
    float value= junk + (float) mp->Rank();
    std::vector<float> vec;
    mp->Gather(value,vec,root);
    // check that they were communicated properly to the root
    if(mp->Rank() == root)
    {
        int nproc = mp->NumberOfProcesses();
        REQUIRE(nproc == (int)vec.size());
        for(int i=0;i<nproc;i++)
            REQUIRE((junk+(float)i) == vec[i]);
    }
}

TEST_CASE("gather vector of ints"){
    int root = 0;
    auto mp = std::make_shared<MessagePasser>();

    std::vector<int> send_vec;
    send_vec.push_back(mp->Rank());
    send_vec.push_back(mp->Rank()+1);
    std::vector<int> recv_vec;
    mp->Gather(send_vec,2,recv_vec,root);

    if(mp->Rank() == root) {
        int nproc = mp->NumberOfProcesses();
        REQUIRE((2*nproc) == (int)recv_vec.size());
        for(int i=0;i<nproc;i++) {
            REQUIRE(i == recv_vec[2*i]);
            REQUIRE((i+1) == recv_vec[2*i+1]);
        }
    }
}
