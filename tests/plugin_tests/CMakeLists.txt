cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitPluginTests)

file(GLOB_RECURSE HEADERS
    "../src/*.h"
    "../src/*.hpp"
)

set(PLUGIN_TESTS_SOURCES
        catch_main.cpp
        ../../src/VizPluginLoader.cpp
        VtkPluginTests.cpp
        )

include_directories(../)

add_executable(PluginTests ${PLUGIN_TESTS_SOURCES} ${HEADERS} ../catch.hpp)
target_link_libraries(PluginTests dl)
