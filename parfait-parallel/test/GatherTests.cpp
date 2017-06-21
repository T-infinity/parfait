#include "MessagePasser.h"
#include "catch.hpp"


TEST_CASE("Gather for integers"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0,value=mp->Rank();
    std::vector<int> vec;
    mp->Gather(value,vec,root);
    // check that they were communicated properly to the root
    if(mp->Rank() == root) {
        int nproc = mp->NumberOfProcesses();
        REQUIRE(nproc == (int)vec.size());
        for(int i=0;i<nproc;i++)
            REQUIRE(i == vec[i]);
    }
}

TEST_CASE("Gather floats"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    float junk = 1.7e-12;
    float value= junk + (float) mp->Rank();
    std::vector<float> vec;
    mp->Gather(value,vec,root);
    // check that they were communicated properly to the root
    if(mp->Rank() == root)
    {
        int nproc = mp->NumberOfProcesses();
        REQUIRE(nproc == (int)vec.size());
        for(int i=0;i<nproc;i++)
            REQUIRE((junk+(float)i) == vec[i]);
    }
}

TEST_CASE("gather vector of ints"){
    int root = 0;
    auto mp = std::make_shared<MessagePasser>();

    std::vector<int> send_vec;
    send_vec.push_back(mp->Rank());
    send_vec.push_back(mp->Rank()+1);
    std::vector<int> recv_vec;
    mp->Gather(send_vec,2,recv_vec,root);

    if(mp->Rank() == root) {
        int nproc = mp->NumberOfProcesses();
        REQUIRE((2*nproc) == (int)recv_vec.size());
        for(int i=0;i<nproc;i++) {
            REQUIRE(i == recv_vec[2*i]);
            REQUIRE((i+1) == recv_vec[2*i+1]);
        }
    }
}
