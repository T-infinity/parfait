
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitParallelTests)

add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

add_definitions(-DPARFAIT_WITH_MPI -DPARFAIT_WITH_PARMETIS -DCATCH_WITH_MPI)
find_package(MPI REQUIRED)

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${MPI_INCLUDE_PATH}"
    )
include_directories(../../tinyxml/src
                    ../)

    file(GLOB_RECURSE HEADERS
        "../../src/*.h"
        "../../src/*.hpp"
        )


set(PARFAIT_PARALLEL_TESTS_SRC
        catch_main.cpp
        ParallelExtent_test.cpp
        parallel_sum_tests.cpp
        GathervTests.cpp
        AllGathervTests.cpp
        PreProcessor/ParallelMeshReaderTests.cpp
        MessageBuilder_test.cpp
        AsyncSends_tests.cpp
        SendRecvScalarTests.cpp
        SendRecvVectorTests.cpp
        SendRecvVectorOfVectorsTests.cpp
        GatherTests.cpp
        SelfSend_test.cpp
        PreProcessor/ParallelNodeToNodeTests.cpp
        PreProcessor/NodeCenteredRedistributorTests.cpp
       )

add_executable(LegacyParallelTests AllTests.cpp message_passer_legacy_tests.cpp)
target_link_libraries(LegacyParallelTests
        CppUTest
        ${MPI_LIBRARIES}
)

add_executable(ParfaitParallelTests ${PARFAIT_PARALLEL_TESTS_SRC} ${HEADERS})
target_link_libraries(ParfaitParallelTests
            ${MPI_LIBRARIES}
            tinyxml
            parmetis
            metis
            )
