
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
#include <MessagePasser/MessagePasser.h>
#include <parfait/SyncPattern.h>
#include <parfait/SyncField.h>

std::map<long, int> buildGlobalToLocal(const std::vector<long>& local_to_global){
    std::map<long, int> global_to_local;
    for(int i = 0; i < local_to_global.size(); i++){
        auto global = local_to_global[i];
        global_to_local[global] = i;
    }
    return global_to_local;
};

class ExampleField {
public:
    double getEntry(long global_id) {
        return data[global_to_local.at(global_id)];
    }

    void setEntry(long global_id, double d) {
        data[global_to_local.at(global_id)] = d;
    }

    std::map<long, int> global_to_local;
    std::vector<double> data;
};

TEST_CASE("Field Exists Tests") {
    MessagePasser mp;
    if(mp.NumberOfProcesses() != 2)
        return;

    ExampleField f;
    f.data.resize(8);
    for(int i = 0; i < 4; i++){
        f.data[i] = 5;
    }
    for(int i = 4; i < 8; i++)
        f.data[i] = -99;
    std::vector<long> global_ids;
    if(mp.Rank() == 0){
        global_ids = std::vector<long>{0,1,2,3,4,5,6,7};
    }
    else if(mp.Rank() == 1){
        global_ids = std::vector<long>{4,5,6,7,0,1,2,3};
    }

    f.global_to_local = buildGlobalToLocal(global_ids);
    auto sync_pattern = Parfait::SyncPattern::build(mp, {global_ids.begin(), global_ids.begin()+4},
                                         {global_ids.begin()+4, global_ids.end()});

    Parfait::syncField<double>(mp, f, sync_pattern);


    REQUIRE(f.data[5] == 5);
}
