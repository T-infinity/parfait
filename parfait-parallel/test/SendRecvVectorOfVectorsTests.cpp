#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

// test Send for vector of vector of integers
TEST_CASE("Vector of vectors"){
    using std::vector;
    vector<vector<int>> vec;
    if(Rank() == 0) {
        vector<int> a = {1,2,3};
        vec.push_back(a);
        vec.push_back(a);
        for(int i=1;i<NumberOfProcesses();i++)
            Send(vec,i);
    }
    else {
        Recv(vec,0);
        REQUIRE(2 == vec.size());
        for(auto row:vec)
            REQUIRE(3 == row.size());
    }
}
