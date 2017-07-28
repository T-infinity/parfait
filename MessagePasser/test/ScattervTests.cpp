#include "../MessagePasser.h"
#include "catch.hpp"

TEST_CASE("Scatterv some integers"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec;
    std::vector<int> recv_vec;
    int length = mp->NumberOfProcesses() + 7;
    if(mp->Rank() == root)
        for(int i=0;i<length;i++)
            vec.push_back(i);
    mp->Scatterv(vec,recv_vec,root);
    if(mp->Rank() < length % mp->NumberOfProcesses())
        REQUIRE((length/mp->NumberOfProcesses()+1) == (int)recv_vec.size());
    else
        REQUIRE((length/mp->NumberOfProcesses()) == (int)recv_vec.size());
    int start = 0;
    for(int i=0;i<mp->Rank();i++){
        if(i < length % mp->NumberOfProcesses())
            start += length/mp->NumberOfProcesses() + 1;
        else
            start += length/mp->NumberOfProcesses();
    }
    for(int i=0;i<(int)recv_vec.size();i++)
        REQUIRE((start+i) == recv_vec[i]);
}
