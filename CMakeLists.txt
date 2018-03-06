cmake_minimum_required(VERSION 3.6)

PROJECT(parfait)
set (CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(BUILD_HEADER_ONLY "To only install Parfait toolkit header files." FALSE)
option(BUILD_TESTS "To build with test support" TRUE)
option(nlohmann_json_path "Specify search path to the nlohmann folder (i.e. the folder containing nlohmann/json.hpp)" "")
option(Catch2_path "Specify search path to catch.hpp" "")


if(BUILD_HEADER_ONLY)
    set(BUILD_TESTS FALSE)
else()
    find_package(MPI REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_path(PARMETIS_INCLUDE_DIRS parmetis.h)
    find_path(METIS_INCLUDE_DIRS metis.h)
    find_library ( PARMETIS_LIBRARY NAMES parmetis)
    find_library ( METIS_LIBRARY NAMES metis)
    find_package_handle_standard_args ( METIS_LIBRARY DEFAULT_MSG METIS_LIBRARY )
    find_package_handle_standard_args ( PARMETIS_LIBRARY DEFAULT_MSG PARMETIS_LIBRARY )
endif()

if(BUILD_TESTS)
    find_package(Catch2 REQUIRED)
endif()

add_subdirectory(MessagePasser)
add_subdirectory(parfait)

if(BUILD_EVERYTHING)
    add_subdirectory(parfait-viz)
    add_subdirectory(utils)
endif()
