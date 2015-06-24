
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitParallelTests)
set(CATCH_DIR "/ump/fldmd/home/cdruyorj/cdruyor_local/catch/include")
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.ugrid")

add_definitions(-DPARFAIT_WITH_MPI)
find_package(MPI REQUIRED)

include_directories(
    "${CATCH_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
    "${MPI_INCLUDE_PATH}"
    )

set(PARFAIT_PARALLEL_TESTS_SRC
        AllTests.cpp  
        message_passer_tests.cpp
        parallel_sum_tests.cpp
       )

add_executable(ParfaitParallelTests ${PARFAIT_PARALLEL_TESTS_SRC})
target_link_libraries(ParfaitParallelTests
            CppUTest
            ${MPI_LIBRARIES}
            )
