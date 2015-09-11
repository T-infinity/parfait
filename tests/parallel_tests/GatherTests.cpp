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

TEST_CASE("gather vector of ints"){
    int root = 0;
    std::vector<int> send_vec;
    send_vec.push_back(Rank());
    send_vec.push_back(Rank()+1);
    std::vector<int> recv_vec;
    Gather(send_vec,2,recv_vec,root);
// check that they were communicated properly to the root
    if(Rank() == root) {
        int nproc = NumberOfProcesses();
        REQUIRE((2*nproc) == (int)recv_vec.size());
        for(int i=0;i<nproc;i++) {
            REQUIRE(i == recv_vec[2*i]);
            REQUIRE((i+1) == recv_vec[2*i+1]);
        }
    }
}
