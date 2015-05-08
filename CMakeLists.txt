cmake_minimum_required(VERSION 2.6)
project(parfait_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(src)

file(GLOB_RECURSE PARFAIT_TEST_SRC
      "test/serial_tests/**.cpp"
      )

include_directories(src)
add_executable(parfait_test ${PARFAIT_TEST_SRC})
target_link_libraries(parfait_test CppUTest parfait_lib)
