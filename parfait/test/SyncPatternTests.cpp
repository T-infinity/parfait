
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
#include <catch.hpp>
#include <vector>
#include <MessagePasser/MessagePasser.h>
#include <map>
#include <parfait/SyncPattern.h>

TEST_CASE("Sync Pattern Exists") {
    MessagePasser mp;

    std::vector<long> have, need;
    if(mp.Rank() == 0){
        have = std::vector<long> {0,1,2,3};
        need = std::vector<long>{4,5,6,7};
    }
    else if(mp.Rank() == 1){
        have = std::vector<long> {4,5,6,7};
        need = std::vector<long> {0,1,2,3};
    }

    Parfait::SyncPattern syncPattern = Parfait::SyncPattern::build(mp, have, need);
    if(mp.NumberOfProcesses() < 2){
        REQUIRE_THROWS(syncPattern.send_to.at(1));
        REQUIRE_THROWS(syncPattern.receive_from.at(1));
    }
    else {
        if(mp.Rank() == 0){
            REQUIRE(syncPattern.send_to.at(1).size() == 4);
            REQUIRE(syncPattern.receive_from.at(1).size() == 4);
        } else if(mp.Rank() == 1){
            REQUIRE(syncPattern.send_to.at(0).size() == 4);
            REQUIRE(syncPattern.receive_from.at(0).size() == 4);
        }
    }
}

TEST_CASE("build send_to_length"){
    MessagePasser mp;
    if(mp.NumberOfProcesses() < 2) return;

    std::vector<long> have, need;
    std::vector<int> need_from;
    if(mp.Rank() == 0){
        have = std::vector<long> {0,1,2,3};
        need = std::vector<long>{4,5,6,7};
        need_from = std::vector<int>{1,1,1,1};
    }
    else if(mp.Rank() == 1){
        have = std::vector<long> {4,5,6,7};
        need = std::vector<long> {0,1,2,3};
        need_from = std::vector<int>{0,0,0,0};
    }

    auto send_to_length = Parfait::SyncPattern::determineHowManyISendToEachRank(mp, need_from);
    REQUIRE(send_to_length.size() == mp.NumberOfProcesses());
    if(mp.Rank() == 0){
        REQUIRE(send_to_length[0] == 0);
        REQUIRE(send_to_length[1] == 4);
    } else if(mp.Rank() == 1){
        REQUIRE(send_to_length[0] == 4);
        REQUIRE(send_to_length[1] == 0);
    }
}

TEST_CASE("Sync Pattern faster build if you know who owns your needs") {
    MessagePasser mp;
    if(mp.NumberOfProcesses() < 2) return;

    std::vector<long> have, need;
    std::vector<int> need_from;
    if(mp.Rank() == 0){
        have = std::vector<long> {0,1,2,3};
        need = std::vector<long>{4,5,6,7};
        need_from = std::vector<int>{1,1,1,1};
    }
    else if(mp.Rank() == 1){
        have = std::vector<long> {4,5,6,7};
        need = std::vector<long> {0,1,2,3};
        need_from = std::vector<int>{0,0,0,0};
    }

    Parfait::SyncPattern syncPattern = Parfait::SyncPattern::build(mp, have, need, need_from);
    if(mp.NumberOfProcesses() < 2){
        REQUIRE_THROWS(syncPattern.send_to.at(1));
        REQUIRE_THROWS(syncPattern.receive_from.at(1));
    }
    else {
        if(mp.Rank() == 0){
            REQUIRE(syncPattern.send_to.at(1).size() == 4);
            REQUIRE(syncPattern.receive_from.at(1).size() == 4);
        } else if(mp.Rank() == 1){
            REQUIRE(syncPattern.send_to.at(0).size() == 4);
            REQUIRE(syncPattern.receive_from.at(0).size() == 4);
        }
    }
}
