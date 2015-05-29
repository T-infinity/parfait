cmake_minimum_required(VERSION 2.8)

project(tinyxml)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
include_directories(src)
add_subdirectory(src)
add_subdirectory(tests)
