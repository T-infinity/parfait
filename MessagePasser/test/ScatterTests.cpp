#include "../MessagePasser.h"
#include "catch.hpp"


TEST_CASE("Scatter integers"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<int> vec;
    int recv_value;
    if(mp->Rank() == root)
        for(int i=0;i<mp->NumberOfProcesses();i++)
            vec.push_back(i);
    mp->Scatter(vec,recv_value,root);
    REQUIRE(mp->Rank() == recv_value);
}

TEST_CASE("Scatter multiple floats"){
    auto mp = std::make_shared<MessagePasser>();

    int root = 0;
    std::vector<float> vec;
    std::vector<float> recv_vec;
    float junk = 1.7e-5;
    if(mp->Rank() == root) {
        for (int i = 0; i < mp->NumberOfProcesses(); i++) {
            vec.push_back(junk + (float) i);
            vec.push_back(junk + (float) (i + 3));
        }
    }
    mp->Scatter(vec,recv_vec,root);
    REQUIRE(2 == (int)recv_vec.size());
    REQUIRE((junk+(float)mp->Rank())  == Approx(recv_vec[0]));
    REQUIRE((junk+(float)(mp->Rank()+3)) == Approx(recv_vec[1]));
}
