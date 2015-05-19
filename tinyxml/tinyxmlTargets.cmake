cmake_minimum_required(VERSION 2.8)

project(tinyxml)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${TINYXML_CMAKE_DIR}/src")
add_subdirectory("${TINYXML_CMAKE_DIR}/src" "${TINYXML_CMAKE_DIR}/build")
