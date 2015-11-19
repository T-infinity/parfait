
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitParallelTests)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

add_definitions(
        -DPARFAIT_WITH_MPI
        -DCATCH_WITH_MPI
)

if(PARFAIT_WITH_PARMETIS)
    add_definitions(
            -DPARFAIT_WITH_PARMETIS
    )
    message("Enabling ParMetis")
else()
    message("Parmetis Disabled")
endif()

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
        DangerStreamTests.cpp
        ParallelExtent_test.cpp
        parallel_sum_tests.cpp
        GathervTests.cpp
        AllGathervTests.cpp
        AsyncSends_tests.cpp
        SendRecvScalarTests.cpp
        SendRecvVectorTests.cpp
        SendRecvVectorOfVectorsTests.cpp
        GatherTests.cpp
        AllGatherTests.cpp
        BroadcastTests.cpp
        ScatterTests.cpp
        ScattervTests.cpp
        ReductionTests.cpp
        MessagePasserBasicsTests.cpp
        SelfSend_test.cpp
        ../serial_tests/StreamTests.cpp
        PreProcessor/ParallelNodeToNodeTests.cpp
        PreProcessor/NodeBasedRedistributorTests.cpp
        PreProcessor/PreProcessorTests.cpp
        PreProcessor/ParallelMeshReaderTests.cpp
        PreProcessor/SendRecvPairTests.cpp
)

add_executable(ParfaitParallelTests
        ${PARFAIT_PARALLEL_TESTS_SRC}
        ${HEADERS})
if(PARFAIT_WITH_PARMETIS)
    target_link_libraries(ParfaitParallelTests
            ${MPI_LIBRARIES}
            tinyxml
            parmetis
            metis
    )
else()
    target_link_libraries(ParfaitParallelTests
            ${MPI_LIBRARIES}
            tinyxml
    )
endif()

