#include "../MessagePasser.h"
#include "catch.hpp"


TEST_CASE("Parallel max of each element in a vector"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec(mp->NumberOfProcesses(),mp->Rank());
    std::vector<int> result = mp->ParallelMax(vec,root);
    if(mp->Rank() == root)
        for(int i=0;i<mp->NumberOfProcesses();i++)
            REQUIRE((mp->NumberOfProcesses()-1) == result[i]);
}

TEST_CASE("Parallel min of each element in a vector"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec(mp->NumberOfProcesses(),mp->Rank()+7);
    std::vector<int> result = mp->ParallelMin(vec,root);
    if(mp->Rank() == root)
        for(int i=0;i<mp->NumberOfProcesses();i++)
            REQUIRE(7 == result[i]);
}

TEST_CASE("Parallel max reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    int value = mp->Rank();
    value = mp->ParallelMax(value, root);
    if(mp->Rank() == root)
        REQUIRE((mp->NumberOfProcesses()-1 == value));
    else
        REQUIRE(value == mp->Rank());
}

TEST_CASE("Parallel min reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = mp->NumberOfProcesses()-1;
    int value = mp->Rank();
    value = mp->ParallelMin(value, root);
    if(mp->Rank() == root)
        REQUIRE(0 == value);
    else
        REQUIRE(value == mp->Rank());
}

TEST_CASE("Parallel rank of max reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = mp->NumberOfProcesses()-1;
    int value = mp->Rank();
    int rank = mp->ParallelRankOfMax(value, root);
    REQUIRE((mp->NumberOfProcesses()-1 == rank));
}

TEST_CASE("Parallel min-all reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int root = mp->NumberOfProcesses()-1;
    int value = mp->Rank();
    value = mp->ParallelMin(value);
    REQUIRE(0 == value);
}

TEST_CASE("Parallel max-all reduction for a single element"){
    auto mp = std::make_shared<MessagePasser>();

    int value = mp->Rank();
    value = mp->ParallelMax(value);
    REQUIRE((mp->NumberOfProcesses()-1 == value));
}

TEST_CASE("Parallel sum of an integer"){
    auto mp = std::make_shared<MessagePasser>();

    auto sum = mp->ParallelSum(1);
    REQUIRE(mp->NumberOfProcesses() == sum);
}

TEST_CASE("Elemental max"){
    auto mp = std::make_shared<MessagePasser>();

    std::vector<int> vec(mp->NumberOfProcesses(),0);
    vec[mp->Rank()] = mp->Rank();
    auto result = mp->AllElementalMax(vec);
    REQUIRE(mp->NumberOfProcesses() == result.size());
    for(int i=0;i<mp->NumberOfProcesses();++i){
        REQUIRE(i == result[i]);
    }
}
