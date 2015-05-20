cmake_minimum_required(VERSION 2.6)
project(parfait_test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(src)
add_subdirectory(tests/serial_tests)


