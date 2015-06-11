cmake_minimum_required(VERSION 2.6)
project(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(mpi_installed true)
set(parmetis_installed true)
set(vtk_installed true)


if(mpi_installed AND parmetis_installed)
    add_definitions(
        -DPARFAIT_WITH_MPI 
        -DPARFAIT_WITH_PARMETIS
        -DCATCH_WITH_MPI)
    find_package(MPI)
    include_directories(${MPI_INCLUDE_PATH})
endif()

include_directories(
    src
    utils
    )


add_subdirectory(src)
add_subdirectory(utils)
add_subdirectory(tests/serial_tests)

if(mpi_installed)
    add_subdirectory(tests/parallel_tests)
endif()
