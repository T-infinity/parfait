#include "MessagePasser.h"
#include "MPICompareTols.h"
#include "catch.hpp"


TEST_CASE("SumIntegers"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0;
    int value = mp->Rank();
    int psum = 0;
    psum = mp->ParallelSum(value,root);
    if(mp->Rank() == root)
    {
        int sum = 0;
        for(int i=0;i<mp->NumberOfProcesses();i++)
            sum += i;
        REQUIRE(sum == psum);
    }
}

TEST_CASE("SumFloats"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0;
    float junk = 1.7e-1;
    float value = junk + (float)mp->Rank();
    float psum = 0.0;
    psum = mp->ParallelSum(value,root);
    if(mp->Rank() == root)
    {
        float sum = 0.0;
        for(int i=0;i< mp->NumberOfProcesses();i++)
            sum += junk + (float)i;
        REQUIRE(sum == Approx(psum));
    }
}

TEST_CASE("SumDoubles"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    double junk = 1.7e-9;
    double value = junk + (double)mp->Rank();
    double psum = 0.0;
    psum = mp->ParallelSum(value,root);
    if(mp->Rank() == root) {
        double sum = 0.0;
        for(int i=0;i<mp->NumberOfProcesses();i++)
            sum += junk + (double)i;
        REQUIRE(sum == Approx(psum));
    }
}
