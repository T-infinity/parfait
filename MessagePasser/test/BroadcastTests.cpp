#include "../MessagePasser.h"
#include "catch.hpp"


TEST_CASE("Broadcast an integer"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,val=0;
    if(mp->Rank() == root)
        val = 5;
    mp->Broadcast(val,root);
    REQUIRE(5 == val);
}

TEST_CASE("Broadcast a double"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    double val=0,check_val=5.0 + 1.7e-13;
    if(mp->Rank() == root)
        val = check_val;
    mp->Broadcast(val,root);
    REQUIRE(check_val == Approx(val));
}

TEST_CASE("Broadcast vector of ints"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,val=0;
    std::vector<int> vec(5,0);
    if(mp->Rank() == root)
        for(int i=0;i<5;i++)
            vec[i] = i;
    mp->Broadcast(vec,5,root);
    REQUIRE(5 == vec.size());
    for(int i=0;i<5;i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("Broadcast string"){
    std::string s = "dog";
    auto mp = MessagePasser();
    mp.Broadcast(s,0);
    REQUIRE(3 == s.size());
}

TEST_CASE("Broadcast a vector of unspecified size"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,val=0;
    std::vector<int> vec;
    if(mp->Rank() == root)
        for(int i=0;i<5;i++)
            vec.push_back(i);
    mp->Broadcast(vec,root);
    REQUIRE(5 == vec.size());
    for(int i=0;i<5;i++)
        REQUIRE(i == vec[i]);
}