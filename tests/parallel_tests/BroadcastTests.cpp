#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("Broadcast an integer"){
    int root = 0,val=0;
    if(Rank() == root)
        val = 5;
    Broadcast(val,root);
    REQUIRE(5 == val);
}

TEST_CASE("Broadcast a double"){
    int root = 0;
    double val=0,check_val=5.0 + 1.7e-13;
    if(Rank() == root)
        val = check_val;
    Broadcast(val,root);
    REQUIRE(check_val == Approx(val));
}

TEST_CASE("Broadcast vector of ints"){
    int root = 0,val=0;
    std::vector<int> vec(5,0);
    if(Rank() == root)
        for(int i=0;i<5;i++)
            vec[i] = i;
    Broadcast(vec,5,root);
    REQUIRE(5 == vec.size());
    for(int i=0;i<5;i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("Broadcast a vector of unspecified size"){
    int root = 0,val=0;
    std::vector<int> vec;
    if(Rank() == root)
        for(int i=0;i<5;i++)
            vec.push_back(i);
    Broadcast(vec,root);
    REQUIRE(5 == vec.size());
    for(int i=0;i<5;i++)
        REQUIRE(i == vec[i]);
}