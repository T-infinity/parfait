#include "MessagePasser.h"
#include "catch.hpp"


TEST_CASE("Scalar integers"){
    auto mp = std::make_shared<MessagePasser>();
    if(mp->Rank() == 0) {
        int value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send(mp->Rank(),0);
    }
}

TEST_CASE("Scalar floats"){
    auto mp = std::make_shared<MessagePasser>();

    if(mp->Rank() == 0) {
        float value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send((float)mp->Rank(),0);
    }
}

TEST_CASE("Scalar doubles"){
    auto mp = std::make_shared<MessagePasser>();

    if(mp->Rank() == 0) {
        double value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send((double)mp->Rank(),0);
    }
}

TEST_CASE("Recv from any source"){
    auto mp = std::make_shared<MessagePasser>();

    if(mp->Rank() == 0) {
        int value = 0,sum=0,check_sum=0;
        for(int i=1;i<mp->NumberOfProcesses();i++) {
            mp->Recv(value);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        mp->Send(mp->Rank(),0);
    }
}