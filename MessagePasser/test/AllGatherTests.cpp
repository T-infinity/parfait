#include "../MessagePasser.h"
#include "catch.hpp"

TEST_CASE("integer scalars"){
    auto mp = std::make_shared<MessagePasser>();
    int root = 0,value=mp->Rank();
    std::vector<int> vec;
    mp->AllGather(value,vec);
    int nproc = mp->NumberOfProcesses();
    REQUIRE(nproc == (int)vec.size());
    for(int i=0;i<nproc;i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("float scalars"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    double junk = 1.7e-13;
    double value=junk + (double)mp->Rank();
    std::vector<double> vec;
    mp->AllGather(value,vec);
    int nproc = mp->NumberOfProcesses();
    REQUIRE(nproc == (int)vec.size());
    for(int i=0;i<nproc;i++)
        REQUIRE((junk+(double)i) == vec[i]);
}


