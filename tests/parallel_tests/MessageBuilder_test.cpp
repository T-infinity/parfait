#include "MessageBuilder.h"
#include <vector>
#include <catch.hpp>

TEST_CASE("Message builder exists"){

    if(MessagePasser::NumberOfProcesses() > 1) {
        MessageBuilder<int> builder;
        if (MessagePasser::Rank() == 0) {
            builder.sendItems({0, 1, 2, -1}, 1);
            builder.finishSends();
        }
        else {
            auto items = builder.recvItemsFrom(0);
            if (MessagePasser::Rank() == 1) {
                REQUIRE(4 == items.size());
            }
        }
    }


}

