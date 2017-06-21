cmake_minimum_required(VERSION 3.0)

PROJECT(Parfait)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(parfait-core)
add_subdirectory(parfait-viz)
add_subdirectory(parfait-utils)
add_subdirectory(parfait-parallel)

