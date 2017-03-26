#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("vector of ints"){
    std::vector<int> vec;
    if(Rank() == 0) {
        for(int i=0;i<9;i++)
            vec.push_back(i);
        for(int i=1;i<NumberOfProcesses();i++)
            Send(vec,9,i);
    }
    else {
        Recv(vec,9,0);
    }
    // check that everyone has the right size vector
    REQUIRE(9 == (int)vec.size());
    for(int i=0;i<(int)vec.size();i++) {
        REQUIRE(i == vec[i]);
    }
}

TEST_CASE("vector of floats"){
    std::vector<float> vec;
    float junk = 1.7e-5;
    if(Rank() == 0) {
        for(int i=0;i<9;i++)
            vec.push_back(junk+(float)i);
        for(int i=1;i<NumberOfProcesses();i++)
            Send(vec,9,i);
    }
    else {
        Recv(vec,9,0);
    }
    // check that everyone has the right size vector
    REQUIRE(9 == (int)vec.size());
    for(int i=0;i<(int)vec.size();i++)
        REQUIRE((junk+(float)i) == Approx(vec[i]));
}

TEST_CASE("Vector of doubles"){
    std::vector<double> vec;
    double junk = 1.7e-13;
    if(Rank() == 0) {
        for(int i=0;i<9;i++)
            vec.push_back(junk+(double)i);
        for(int i=1;i<NumberOfProcesses();i++)
            Send(vec,9,i);
    }
    else {
        Recv(vec,9,0);
    }
    // check that everyone has the right size vector
    REQUIRE(9 == (int)vec.size());
    for(int i=0;i<(int)vec.size();i++)
        REQUIRE((junk+(double)i) == Approx(vec[i]));
}

TEST_CASE("vector of integers whose size is unknown to receiver"){
    std::vector<int> vec;
    if(Rank() == 0) {
        for(int i=0;i<9;i++)
            vec.push_back(i);
        for(int i=1;i<NumberOfProcesses();i++)
            Send(vec,i);
    }
    else {
        Recv(vec,0);
    }
    // check that everyone has the right size vector
    REQUIRE(9 == (int)vec.size());
    for(int i=0;i<(int)vec.size();i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("Empty vector"){
    std::vector<int> vec;
    if(Rank() == 0)
        for(int i=1;i<NumberOfProcesses();i++)
            Send(vec,i);
    else
        Recv(vec,0);
    // check that everyone has the right size vector
    REQUIRE(0 == (int)vec.size());
}

