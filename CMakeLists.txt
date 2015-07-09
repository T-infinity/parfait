cmake_minimum_required(VERSION 2.6)
project(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    src
    utils
    )

add_subdirectory(utils)
add_subdirectory(tests/serial_tests)
add_subdirectory(tests/parallel_tests)
