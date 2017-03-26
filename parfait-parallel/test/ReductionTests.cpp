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

TEST_CASE("Parallel max reduction for a single element"){
    int root = 0;
    int value = MessagePasser::Rank();
    value = MessagePasser::ParallelMax(value, root);
    if(MessagePasser::Rank() == root)
        REQUIRE((MessagePasser::NumberOfProcesses()-1 == value));
    else
        REQUIRE(value == MessagePasser::Rank());
}

TEST_CASE("Parallel min reduction for a single element"){
    int root = MessagePasser::NumberOfProcesses()-1;
    int value = MessagePasser::Rank();
    value = MessagePasser::ParallelMin(value, root);
    if(MessagePasser::Rank() == root)
        REQUIRE(0 == value);
    else
        REQUIRE(value == MessagePasser::Rank());
}

TEST_CASE("Parallel min-all reduction for a single element"){
    int root = MessagePasser::NumberOfProcesses()-1;
    int value = MessagePasser::Rank();
    value = MessagePasser::ParallelMinAll(value);
    REQUIRE(0 == value);
}

TEST_CASE("Parallel max-all reduction for a single element"){
    int value = MessagePasser::Rank();
    value = MessagePasser::ParallelMaxAll(value);
    REQUIRE((MessagePasser::NumberOfProcesses()-1 == value));
}

TEST_CASE("Parallel sum of an integer"){
    auto sum = MessagePasser::ParallelSum(1);
    REQUIRE(MessagePasser::NumberOfProcesses() == sum);
}

TEST_CASE("Elemental max"){
    std::vector<int> vec(NumberOfProcesses(),0);
    vec[Rank()] = Rank();
    auto result = AllElementalMax(vec);
    REQUIRE(NumberOfProcesses() == result.size());
    for(int i=0;i<NumberOfProcesses();++i){
        REQUIRE(i == result[i]);
    }
}