#include <vector>
#include "../MessagePasser.h"
#include <catch.hpp>

TEST_CASE("TestWithIntegers"){
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
    mp->AllGatherv(send_vec,recv_vec,map);
    REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
    REQUIRE(map.back() == (int)recv_vec.size());
    for(int i=0;i< mp->NumberOfProcesses();i++){
        int counter = 0;
        for(int j=map[i];j<map[i+1];j++){
            REQUIRE((i+counter) == recv_vec[j]);
            ++counter;
        }
    }
}

TEST_CASE("TestWithNoMapPassedFromRoot"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> send_vec;
    std::vector<int> recv_vec;
    std::vector<int> map(mp->NumberOfProcesses()+1);
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if(mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for(int i=0;i<num;i++)
        send_vec.push_back(mp->Rank()+i);
    mp->AllGatherv(send_vec,recv_vec);

    for(int i=1;i<mp->NumberOfProcesses()+1;i++){
        num = (i-1)%2==0 ? 2 : 3;
        map[i] = map[i-1] + num;
    }
    REQUIRE((mp->NumberOfProcesses()+1) == (int)map.size());
    REQUIRE(map.back() == (int)recv_vec.size());
    for(int i=0;i<mp->NumberOfProcesses();i++) {
        int counter = 0;
        for(int j=map[i];j<map[i+1];j++) {
            REQUIRE((i+counter) == recv_vec[j]);
            ++counter;
        }
    }
}

TEST_CASE("TestWithVectorOfVectors"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector <int> send_vec;
    std::vector <std::vector<int>> result;
    std::vector <int> map(mp->NumberOfProcesses() + 1);
    int num = 0;
    // even and odd procs have size 2 & 3 vectors respectively
    if (mp->Rank() % 2 == 0)
        num = 2;
    else
        num = 3;
    for (int i = 0; i < num; i++)
        send_vec.push_back(mp->Rank() + i);
    mp->AllGatherv(send_vec, result);
    for(int i=0;i<mp->NumberOfProcesses();i++){
        if(i % 2 == 0) {
            REQUIRE(2 == result[i].size());
            for(int j=0;j<2;j++)
                REQUIRE((i+j) == result[i][j]);
        }
        else{
            REQUIRE(3 == result[i].size());
            for(int j=0;j<3;j++)
                REQUIRE((i+j) == result[i][j]);
        }

    }
}
