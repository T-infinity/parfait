cmake_minimum_required(VERSION 2.6)
project(parfait_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE PARFAIT_SRC
      "*.cpp"
      )

add_library(parfait_lib ${PARFAIT_SRC})
