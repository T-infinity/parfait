cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitPluginTests)

file(GLOB_RECURSE HEADERS
    "../../core/*.h"
    "../../core/*.hpp"
)

set(PLUGIN_TESTS_SOURCES
        ${CATCH_DIR}/catch_main.cpp
        ../../core/VizPluginLoader.cpp
        VtkPluginTests.cpp
        )

include_directories(../ ${CATCH_DIR})

add_executable(PluginTests ${PLUGIN_TESTS_SOURCES} ${HEADERS})
target_link_libraries(PluginTests dl)
