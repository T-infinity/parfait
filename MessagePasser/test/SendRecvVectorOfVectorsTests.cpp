
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


// test Send for vector of vector of integers
TEST_CASE("Vector of vectors"){
    auto mp = std::make_shared<MessagePasser>();

    using std::vector;
    vector<vector<int>> vec;
    if(mp->Rank() == 0) {
        vector<int> a = {1,2,3};
        vec.push_back(a);
        vec.push_back(a);
        for(int i=1;i<mp->NumberOfProcesses();i++)
            mp->Send(vec,i);
    }
    else {
        mp->Recv(vec,0);
        REQUIRE(2 == vec.size());
        for(auto row:vec)
            REQUIRE(3 == row.size());
    }
}
