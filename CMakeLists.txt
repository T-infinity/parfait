cmake_minimum_required(VERSION 2.6)
project(parfait)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE PARFAIT_SRC
      "src/*.h"
      "src/*.hpp"
      "src/*.cpp"
      "test/serial_tests/**.h"
      "test/serial_tests/**.hpp"
      "test/serial_tests/**.cpp"
      )

include_directories(src)
add_executable(parfait ${PARFAIT_SRC})
target_link_libraries(parfait CppUTest)
