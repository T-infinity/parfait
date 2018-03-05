
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

TEST_CASE("Scatterv some integers"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec;
    std::vector<int> recv_vec;
    int length = mp->NumberOfProcesses() + 7;
    if(mp->Rank() == root)
        for(int i=0;i<length;i++)
            vec.push_back(i);
    mp->Scatterv(vec,recv_vec,root);
    if(mp->Rank() < length % mp->NumberOfProcesses())
        REQUIRE((length/mp->NumberOfProcesses()+1) == (int)recv_vec.size());
    else
        REQUIRE((length/mp->NumberOfProcesses()) == (int)recv_vec.size());
    int start = 0;
    for(int i=0;i<mp->Rank();i++){
        if(i < length % mp->NumberOfProcesses())
            start += length/mp->NumberOfProcesses() + 1;
        else
            start += length/mp->NumberOfProcesses();
    }
    for(int i=0;i<(int)recv_vec.size();i++)
        REQUIRE((start+i) == recv_vec[i]);
}
