#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("Gather for integers"){
    int root = 0,value=Rank();
    std::vector<int> vec;
    Gather(value,vec,root);
    // check that they were communicated properly to the root
    if(Rank() == root) {
        int nproc = NumberOfProcesses();
        REQUIRE(nproc == (int)vec.size());
        for(int i=0;i<nproc;i++)
            REQUIRE(i == vec[i]);
    }
}

TEST_CASE("Gather floats"){
    int root = 0;
    float junk = 1.7e-12;
    float value= junk + (float) Rank();
    std::vector<float> vec;
    Gather(value,vec,root);
    // check that they were communicated properly to the root
    if(Rank() == root)
    {
        int nproc = NumberOfProcesses();
        REQUIRE(nproc == (int)vec.size());
        for(int i=0;i<nproc;i++)
            REQUIRE((junk+(float)i) == vec[i]);
    }
}
