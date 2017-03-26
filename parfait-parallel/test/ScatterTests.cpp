#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("Scatter integers"){
    int root = 0;
    std::vector<int> vec;
    int recv_value;
    if(Rank() == root)
        for(int i=0;i<NumberOfProcesses();i++)
            vec.push_back(i);
    Scatter(vec,recv_value,root);
    REQUIRE(Rank() == recv_value);
}

TEST_CASE("Scatter multiple floats"){
    int root = 0;
    std::vector<float> vec;
    std::vector<float> recv_vec;
    float junk = 1.7e-5;
    if(Rank() == root) {
        for (int i = 0; i < NumberOfProcesses(); i++) {
            vec.push_back(junk + (float) i);
            vec.push_back(junk + (float) (i + 3));
        }
    }
    Scatter(vec,recv_vec,root);
    REQUIRE(2 == (int)recv_vec.size());
    REQUIRE((junk+(float)Rank())  == Approx(recv_vec[0]));
    REQUIRE((junk+(float)(Rank()+3)) == Approx(recv_vec[1]));
}
