#include <catch.hpp>

#include <memory>

using namespace Parfait;
class ParallelArray {
public:
    ParallelArray(std::shared_ptr<SendReceivePair> pair);
private:
    std::shared_ptr<SendReceivePair> srPair;
};

SendReceivePair getSimpleSendRecvPair() {
    SendReceivePair sendReceivePair;
    if(MessagePasser::Rank() == 0) {
        sendReceivePair.send[0] = std::vector<int>{1};
        sendReceivePair.send[1] = std::vector<int>{1};
        sendReceivePair.send[2] = std::vector<int>{1};
        sendReceivePair.send[3] = std::vector<int>{};
        sendReceivePair.send[4] = std::vector<int>{};
        sendReceivePair.send[5] = std::vector<int>{};
        sendReceivePair.recv = {-1,-1,-1,1,1,1};
    }
    if(MessagePasser::Rank() == 1) {
        sendReceivePair.send[0] = std::vector<int>{};
        sendReceivePair.send[1] = std::vector<int>{};
        sendReceivePair.send[2] = std::vector<int>{};
        sendReceivePair.send[3] = std::vector<int>{0};
        sendReceivePair.send[4] = std::vector<int>{0};
        sendReceivePair.send[5] = std::vector<int>{0};
        sendReceivePair.recv = {0,0,0,-1,-1,-1};
    }
    return sendReceivePair;
};

TEST_CASE("Parallel Array Exists"){
    auto srPair = std::make_shared<SendReceivePair>(getSimpleSendRecvPair());
    ParallelArray v(srPair);
}