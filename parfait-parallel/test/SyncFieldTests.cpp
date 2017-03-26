#include <catch.hpp>
#include <MessagePasser.h>
#include <SyncPattern.h>
#include <SyncField.h>

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
    if(MessagePasser::NumberOfProcesses() != 2)
        return;

    ExampleField f;
    f.data.resize(8);
    for(int i = 0; i < 4; i++){
        f.data[i] = 5;
    }
    for(int i = 4; i < 8; i++)
        f.data[i] = -99;
    std::vector<long> global_ids;
    if(MessagePasser::Rank() == 0){
        global_ids = std::vector<long>{0,1,2,3,4,5,6,7};
    }
    else if(MessagePasser::Rank() == 1){
        global_ids = std::vector<long>{4,5,6,7,0,1,2,3};
    }

    f.global_to_local = buildGlobalToLocal(global_ids);
    auto sync_pattern = buildSyncPattern({global_ids.begin(), global_ids.begin()+4},
                                         {global_ids.begin()+4, global_ids.end()});

    syncField<double>(f, sync_pattern);


    REQUIRE(f.data[5] == 5);
}
