cmake_minimum_required(VERSION 3.6)

PROJECT(parfait)
set (CMAKE_CXX_STANDARD 14)

set(BUILD_EVERYTHING TRUE CACHE BOOL "dogs...")

if(BUILD_EVERYTHING)
    find_package(MPI REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_path(PARMETIS_INCLUDE_DIRS parmetis.h)
    find_path(METIS_INCLUDE_DIRS metis.h)
    find_library ( PARMETIS_LIBRARY NAMES parmetis)
    find_library ( METIS_LIBRARY NAMES metis)
    find_package_handle_standard_args ( METIS_LIBRARY DEFAULT_MSG METIS_LIBRARY )
    find_package_handle_standard_args ( PARMETIS_LIBRARY DEFAULT_MSG PARMETIS_LIBRARY )

    # Prepare "Catch" library for other executables
    set(CATCH_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/catch)
    add_library(Catch INTERFACE)
    target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
endif()

add_subdirectory(MessagePasser)
add_subdirectory(parfait)

if(BUILD_EVERYTHING)
    add_subdirectory(parfait-viz)
    add_subdirectory(utils)
endif()
