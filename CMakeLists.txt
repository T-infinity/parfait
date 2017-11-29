cmake_minimum_required(VERSION 3.6)

PROJECT(parfait)
set (CMAKE_CXX_STANDARD 14)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

add_subdirectory(MessagePasser)
add_subdirectory(parfait)
add_subdirectory(parfait-viz)
add_subdirectory(utils)
