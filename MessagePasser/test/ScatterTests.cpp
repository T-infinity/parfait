
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


TEST_CASE("Scatter integers"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec;
    int recv_value;
    if(mp->Rank() == root)
        for(int i=0;i<mp->NumberOfProcesses();i++)
            vec.push_back(i);
    mp->Scatter(vec,recv_value,root);
    REQUIRE(mp->Rank() == recv_value);
}

TEST_CASE("Scatter multiple floats"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<float> vec;
    std::vector<float> recv_vec;
    float junk = 1.7e-5;
    if(mp->Rank() == root) {
        for (int i = 0; i < mp->NumberOfProcesses(); i++) {
            vec.push_back(junk + (float) i);
            vec.push_back(junk + (float) (i + 3));
        }
    }
    mp->Scatter(vec,recv_vec,root);
    REQUIRE(2 == (int)recv_vec.size());
    REQUIRE((junk+(float)mp->Rank())  == Approx(recv_vec[0]));
    REQUIRE((junk+(float)(mp->Rank()+3)) == Approx(recv_vec[1]));
}
