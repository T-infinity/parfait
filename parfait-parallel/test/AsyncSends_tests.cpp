#include <vector>
#include <MessagePasser.h>
#include "catch.hpp"
#include <Checkpoint.h>


TEST_CASE("to test async send (and WaitAll)"){
    auto mp = std::make_shared<MessagePasser>();
    std::vector<int> vec(300000,0);
    std::vector<MessagePasser::MessageStatus> statuses;
    if(mp->Rank() == 0){
        for(int i=0;i<mp->NumberOfProcesses();i++){
            statuses.push_back(mp->NonBlockingSend(vec,i));
        }
    }
    std::vector<int> recv_vec;
    mp->Recv(recv_vec,0);
    if(mp->Rank() == 0)
        mp->WaitAll(statuses);
    REQUIRE(300000 == recv_vec.size());
}

