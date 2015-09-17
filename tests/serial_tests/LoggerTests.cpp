#include <catch.hpp>
#include <Logger.h>

using namespace Parfait;

TEST_CASE("Logger Exists"){
    Logger::log("hello log %d", 5);
    Logger::setFileName("cool-output.txt");
    Logger::log("hello log 6");
    Logger::setOutputStream(stdout);
    Logger::log("hello log 7");
}

