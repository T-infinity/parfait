#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("Parallel max of each element in a vector"){
    int root = 0;
    std::vector<int> vec(NumberOfProcesses(),Rank());
    std::vector<int> result = ParallelMax(vec,root);
    if(Rank() == root)
        for(int i=0;i<NumberOfProcesses();i++)
            REQUIRE((NumberOfProcesses()-1) == result[i]);
}

TEST_CASE("Parallel min of each element in a vector"){
    int root = 0;
    std::vector<int> vec(NumberOfProcesses(),Rank()+7);
    std::vector<int> result = ParallelMin(vec,root);
    if(Rank() == root)
        for(int i=0;i<NumberOfProcesses();i++)
            REQUIRE(7 == result[i]);
}

TEST_CASE("Parallel sum of an integer"){
    auto sum = MessagePasser::ParallelSum(1);
    REQUIRE(MessagePasser::NumberOfProcesses() == sum);
}

