#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("Scalar integers"){
    if(Rank() == 0) {
        int value = 0,sum=0,check_sum=0;
        for(int i=1;i<NumberOfProcesses();i++) {
            Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        Send(Rank(),0);
    }
}

TEST_CASE("Scalar floats"){
    if(Rank() == 0) {
        float value = 0,sum=0,check_sum=0;
        for(int i=1;i<NumberOfProcesses();i++) {
            Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        Send((float)Rank(),0);
    }
}

TEST_CASE("Scalar doubles"){
    if(Rank() == 0) {
        double value = 0,sum=0,check_sum=0;
        for(int i=1;i<NumberOfProcesses();i++) {
            Recv(value,i);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        Send((double)Rank(),0);
    }
}

TEST_CASE("Recv from any source"){
    if(Rank() == 0) {
        int value = 0,sum=0,check_sum=0;
        for(int i=1;i<NumberOfProcesses();i++) {
            Recv(value);
            sum += value;
            check_sum += i;
        }
        REQUIRE(check_sum == sum);
    }
    else {
        Send(Rank(),0);
    }
}