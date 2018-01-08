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
