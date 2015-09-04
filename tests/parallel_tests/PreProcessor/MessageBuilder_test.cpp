#include "MessageBuilder.h"
#include <vector>
#include <catch.hpp>

TEST_CASE("Message builder exists"){

    MessageBuilder<int> builder;
    if(MessagePasser::Rank() == 0) {
        builder.sendItems({0, 1, 2, -1}, 0);
        builder.finishSends();
    }
    auto items = builder.recvItemsFrom(0);
    if(MessagePasser::Rank() == 0)
        REQUIRE(4 == items.size());
}

