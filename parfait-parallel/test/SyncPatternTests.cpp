#include <catch.hpp>
#include <vector>
#include <MessagePasser.h>
#include <map>
#include <SyncPattern.h>

TEST_CASE("Sync Pattern Exists") {
    std::vector<long> have, need;
    if(MessagePasser::Rank() == 0){
        have = std::vector<long> {0,1,2,3};
        need = std::vector<long>{4,5,6,7};
    }
    else if(MessagePasser::Rank() == 1){
        have = std::vector<long> {4,5,6,7};
        need = std::vector<long> {0,1,2,3};
    }

    SyncPattern syncPattern = buildSyncPattern(have, need);
    if(MessagePasser::NumberOfProcesses() < 2){
        REQUIRE_THROWS(syncPattern.send_to.at(1));
        REQUIRE_THROWS(syncPattern.receive_from.at(1));
    }
    else {
        if(MessagePasser::Rank() == 0){
            REQUIRE(syncPattern.send_to.at(1).size() == 4);
            REQUIRE(syncPattern.receive_from.at(1).size() == 4);
        } else if(MessagePasser::Rank() == 1){
            REQUIRE(syncPattern.send_to.at(0).size() == 4);
            REQUIRE(syncPattern.receive_from.at(0).size() == 4);
        }
    }
}
