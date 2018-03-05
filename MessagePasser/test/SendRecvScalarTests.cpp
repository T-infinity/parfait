
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


TEST_CASE("Scalar integers"){
    auto mp = std::make_shared<MessagePasser>();
    if(mp->Rank() == 0) {
        int value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send(mp->Rank(),0);
    }
}

TEST_CASE("Scalar floats"){
    auto mp = std::make_shared<MessagePasser>();

    if(mp->Rank() == 0) {
        float value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send((float)mp->Rank(),0);
    }
}

TEST_CASE("Scalar doubles"){
    auto mp = std::make_shared<MessagePasser>();

    if(mp->Rank() == 0) {
        double value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send((double)mp->Rank(),0);
    }
}

TEST_CASE("Recv from any source"){
    auto mp = std::make_shared<MessagePasser>();

    if(mp->Rank() == 0) {
        int value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send(mp->Rank(),0);
    }
}