#include <catch.hpp>
#include <algorithm>
#include <MessagePasser/MessagePasser.h>

TEST_CASE("Message Passer Tests"){
    std::vector<MessagePasser::MessageStatus> statii(10000);
    auto* ptr1 = statii.front().request();
    statii.back() = statii.front();

    REQUIRE(ptr1 == statii.back().request());
}
