#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("Scatterv some integers"){
    int root = 0;
    std::vector<int> vec;
    std::vector<int> recv_vec;
    int length = NumberOfProcesses() + 7;
    if(Rank() == root)
        for(int i=0;i<length;i++)
            vec.push_back(i);
    Scatterv(vec,recv_vec,root);
// check that each proc has the right size vector after the call
    if(Rank() < length % NumberOfProcesses())
        REQUIRE((length/NumberOfProcesses()+1) == (int)recv_vec.size());
    else
        REQUIRE((length/NumberOfProcesses()) == (int)recv_vec.size());
// check that each proc has the correct range of values
    int start = 0;
    for(int i=0;i<Rank();i++){
        if(i < length % NumberOfProcesses())
            start += length/NumberOfProcesses() + 1;
        else
            start += length/NumberOfProcesses();
    }
    for(int i=0;i<(int)recv_vec.size();i++)
        REQUIRE((start+i) == recv_vec[i]);
}
