#include "MessageBuilder.h"
#include <vector>
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(MessageBuilderTests){};

TEST(MessageBuilderTests, Exists){

    MessageBuilder<int> builder;
    builder.sendItems({0, 1, 2, -1}, 0);
    builder.finishSends();
    auto items = builder.recvItemsFrom(0);
    LONGS_EQUAL(4, items.size());
}

