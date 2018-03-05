
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
#include <parfait/Checkpoint.h>


TEST_CASE("to test async send (and WaitAll)"){
    auto mp = std::make_shared<MessagePasser>();
    std::vector<int> vec(300000,0);
    std::vector<MessagePasser::MessageStatus> statuses;
    if(mp->Rank() == 0){
        for(int i=0;i<mp->NumberOfProcesses();i++){
            statuses.push_back(mp->NonBlockingSend(vec,i));
        }
    }
    std::vector<int> recv_vec;
    mp->Recv(recv_vec,0);
    if(mp->Rank() == 0)
        mp->WaitAll(statuses);
    REQUIRE(300000 == recv_vec.size());
}

