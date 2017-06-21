#include <vector>
#include <MessagePasser.h>
#include "MPICompareTols.h"
#include "catch.hpp"

TEST_CASE("Test Gatherv for integers"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    std::vector<int> map;
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(mp->Rank()+i);
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(map.back() == (int)recv_vec.size());
        for(int i = 0; i < mp->NumberOfProcesses(); i++) {
            int counter = 0;
            for(int j = map[i]; j < map[i+1]; j++) {
                REQUIRE((i+counter) == recv_vec[j]);
                ++counter;
            }
        }
    }

}

TEST_CASE("TestWithIntegersNoMapPassedIn"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(mp->Rank()+i);
    mp->Gatherv(send_vec,recv_vec,root);
    // just make sure it gets called, since it is just a delegation
}

TEST_CASE("TestWithFloats"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<float> send_vec;
    std::vector<float> recv_vec;
    float junk = 1.7e-5;
    std::vector<int> map;
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(junk+(float)(mp->Rank()+i));
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(map.back() == (int)recv_vec.size());
        for(int i=0;i<mp->NumberOfProcesses();i++) {
            int counter = 0;
            for(int j=map[i];j<map[i+1];j++) {
                REQUIRE((junk+(float)(i+counter)) == Approx(recv_vec[j]));
                ++counter;
            }
        }
    }
}

TEST_CASE("TestWithDoubles"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<double> send_vec;
    std::vector<double> recv_vec;
    double junk = 1.7e-13;
    std::vector<int> map;
    int num = 0;
// even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(junk+(double)(mp->Rank()+i));
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(map.back() == (int)recv_vec.size());
        for(int i=0;i<mp->NumberOfProcesses();i++) {
            int counter = 0;
            for(int j=map[i];j<map[i+1];j++) {
                REQUIRE((junk+(double)(i+counter)) == Approx(recv_vec[j]));
                ++counter;
            }
        }
    }
}

TEST_CASE("TestWhenAllVectorsAreEmpty"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    std::vector<int> map;
    mp->Gatherv(send_vec,recv_vec,map,root);
    if(mp->Rank() == root) {
        REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
        REQUIRE(0 == (int)recv_vec.size());
        REQUIRE(0 == map.back());
    }
}

TEST_CASE("TestGatheringAsVectorOfVectors"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<double> send_vec;
    std::vector<std::vector<double>> result;
    double junk = 1.7e-13;
    std::vector<int> map;
    int num = 0;
// even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(junk+(double)(mp->Rank()+i));
    mp->Gatherv(send_vec, result,root);
    if(mp->Rank() == root) {
        REQUIRE(mp->NumberOfProcesses() == (int) result.size());
        for(int i=0;i<mp->NumberOfProcesses();i++) {
            int counter = 0;
            REQUIRE((i%2==0?2:3) == result[i].size());
            for(unsigned int j=0;j< result[i].size();j++) {
                REQUIRE((junk+(double)(i+counter)) == Approx(result[i][j]));
                ++counter;
            }
        }
    }
}
