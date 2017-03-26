#include <vector>
#include <MessagePasser.h>
#include "catch.hpp"
#include <Checkpoint.h>

using namespace MessagePasser;

TEST_CASE("to test async send (and WaitAll)"){
    std::vector<int> vec(300000,0);
    std::vector<MessageStatus> statuses;
    if(Rank() == 0){
        for(int i=0;i<NumberOfProcesses();i++){
            statuses.push_back(NonBlockingSend(vec,i));
        }
    }
    std::vector<int> recv_vec;
    Recv(recv_vec,0);
    if(Rank() == 0)
        WaitAll(statuses);
    REQUIRE(300000 == recv_vec.size());
}

