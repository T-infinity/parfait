#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("integer scalars"){
    int root = 0,value=Rank();
    std::vector<int> vec;
    AllGather(value,vec);
// check that they were communicated properly to everyone
    int nproc = NumberOfProcesses();
    REQUIRE(nproc == (int)vec.size());
    for(int i=0;i<nproc;i++)
        REQUIRE(i == vec[i]);
}

TEST_CASE("float scalars"){
    int root = 0;
    double junk = 1.7e-13;
    double value=junk + (double)Rank();
    std::vector<double> vec;
    AllGather(value,vec);
// check that they were communicated properly to everyone
    int nproc = NumberOfProcesses();
    REQUIRE(nproc == (int)vec.size());
    for(int i=0;i<nproc;i++)
        REQUIRE((junk+(double)i) == vec[i]);
}


