cmake_minimum_required(VERSION 2.8)

PROJECT(Parfait)
set (CMAKE_CXX_STANDARD 14)

add_subdirectory(parfait-core)
add_subdirectory(parfait-parallel)
add_subdirectory(parfait-viz)
add_subdirectory(parfait-utils)

