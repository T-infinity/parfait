#include "MessageBuilder.h"
#include <vector>
#include <catch.hpp>

TEST_CASE("Message builder exists and can stuff"){

    MessageBuilder<int> builder;
    builder.sendItems({0, 1, 2, -1}, 0);
    builder.finishSends();
    if(MessagePasser::Rank() == 0) {
        auto items = builder.recvItemsFrom(0);
        REQUIRE(4 == items.size());
    }
}

