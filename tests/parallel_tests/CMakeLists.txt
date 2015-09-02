
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitParallelTests)

add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

add_definitions(-DPARFAIT_WITH_MPI -DPARFAIT_WITH_PARMETIS)
find_package(MPI REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${MPI_INCLUDE_PATH}"
    )
include_directories(../../tinyxml/src)

    file(GLOB_RECURSE HEADERS
        "../../src/*.h"
        "../../src/*.hpp"
        )

set(PARFAIT_PARALLEL_TESTS_SRC
        AllTests.cpp
        ParallelExtent_test.cpp
        ParallelImportedUgrid_test.cpp
        message_passer_tests.cpp
        parallel_sum_tests.cpp
        GathervTests.cpp
        AllGathervTests.cpp
        PreProcessor/ParallelMeshReaderTests.cpp
        PreProcessor/MessageBuilder_test.cpp
       )

add_executable(ParfaitParallelTests ${PARFAIT_PARALLEL_TESTS_SRC} ${HEADERS})
target_link_libraries(ParfaitParallelTests
            CppUTest
            ${MPI_LIBRARIES}
            tinyxml
            parmetis
            metis
            )
