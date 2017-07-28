#include <catch.hpp>
#include <vector>
#include <MessagePasser.h>
#include <map>
#include <SyncPattern.h>

TEST_CASE("Sync Pattern Exists") {
    auto mp = std::make_shared<MessagePasser>();

    std::vector<long> have, need;
    if(mp->Rank() == 0){
        have = std::vector<long> {0,1,2,3};
        need = std::vector<long>{4,5,6,7};
    }
    else if(mp->Rank() == 1){
        have = std::vector<long> {4,5,6,7};
        need = std::vector<long> {0,1,2,3};
    }

    Parfait::SyncPattern syncPattern = Parfait::SyncPattern::build(mp, have, need);
    if(mp->NumberOfProcesses() < 2){
        REQUIRE_THROWS(syncPattern.send_to.at(1));
        REQUIRE_THROWS(syncPattern.receive_from.at(1));
    }
    else {
        if(mp->Rank() == 0){
            REQUIRE(syncPattern.send_to.at(1).size() == 4);
            REQUIRE(syncPattern.receive_from.at(1).size() == 4);
        } else if(mp->Rank() == 1){
            REQUIRE(syncPattern.send_to.at(0).size() == 4);
            REQUIRE(syncPattern.receive_from.at(0).size() == 4);
        }
    }
}
