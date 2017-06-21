cmake_minimum_required(VERSION 3.0)

PROJECT(ParfaitParallelTests)
include ( FindPackageHandleStandardArgs )
set (CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)
find_package(nlohmann_json REQUIRED)
find_path(PARMETIS_INCLUDE_DIRS parmetis.h)
find_path(METIS_INCLUDE_DIRS metis.h)
find_library ( PARMETIS_LIBRARY NAMES parmetis)
find_library ( METIS_LIBRARY NAMES metis)
find_package_handle_standard_args ( METIS_LIBRARY DEFAULT_MSG METIS_LIBRARY )
find_package_handle_standard_args ( PARMETIS_LIBRARY DEFAULT_MSG PARMETIS_LIBRARY )
if(NOT PARMETIS_LIBRARY)
  message(FATAL_ERROR "parmetis not found")
endif()

if(NOT METIS_LIBRARY)
  message(FATAL_ERROR "metis not found")
endif()

add_definitions(
  -DPARFAIT_WITH_PARMETIS
  -DCATCH_WITH_MPI
)

add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/grids/6cell.lb8.ugrid")
add_definitions(-DSIX_CELL_TEST_JSON="${CMAKE_SOURCE_DIR}/grids/6cell.json")
add_definitions(-DRAMP_TEST_JSON="${CMAKE_SOURCE_DIR}/grids/ramp.json")
add_definitions(-DCOMBINED_TEST_JSON="${CMAKE_SOURCE_DIR}/grids/combined.json")


include_directories(
        ../parfait-core/src/
        src
        test
        ${PARMETIS_INCLUDE_DIRS}
        ${METIS_INCLUDE_DIRS}
        ${MPI_INCLUDE_PATH}
        ${JSON_INCLUDE_DIR}
)

file(GLOB_RECURSE PARFAIT_HEADERS
        ../parfait-core/src/*.h
        ../parfait-core/src/*.hpp
        ../parfait-parallel/src/*.h
        ../parfait-parallel/src/*.hpp
        src/*.h
        src/*.hpp
)

file(GLOB_RECURSE MESSAGE_PASSER_HEADERS
        src/Mess*.h
        src/Mess*.hpp
)

set(SOURCE_FILES
    src/Helpers.cpp
    src/SurfaceElementWinding.cpp
    src/SurfaceNeighbors.cpp
    src/SyncPattern.cpp
    )

set(PARFAIT_PARALLEL_TESTS_SRC
        test/AllTests.cpp
        test/ParallelExtent_test.cpp
        test/ParallelSumTests.cpp
        test/AllGathervTests.cpp
        test/AllGatherTests.cpp
        test/GathervTests.cpp
        test/GridSystemPreProcessorTests.cpp
        test/AsyncSends_tests.cpp
        test/SendRecvScalarTests.cpp
        test/SendRecvVectorTests.cpp
        test/SyncFieldTests.cpp
        test/SyncPatternTests.cpp
        test/SendRecvVectorOfVectorsTests.cpp
        test/GatherTests.cpp
        test/BroadcastTests.cpp
        test/ScatterTests.cpp
        test/ScattervTests.cpp
        test/ReductionTests.cpp
        test/MessagePasserBasicsTests.cpp
        test/SelfSend_test.cpp
        test/ParallelNodeToNodeTests.cpp
        test/NodeBasedRedistributorTests.cpp
        test/PreProcessorTests.cpp
        test/ParallelMeshReaderTests.cpp
        src/MessagePasser.cpp
)

add_library(MessagePasser src/MessagePasser.cpp ${SOURCE_FILES} ${PARFAIT_HEADERS})

add_executable(ParfaitParallelTests
        ${PARFAIT_PARALLEL_TESTS_SRC}
        ${HEADERS})

target_link_libraries(ParfaitParallelTests
        MessagePasser
        ${MPI_LIBRARIES}
        ${PARMETIS_LIBRARY}
        ${METIS_LIBRARY}
)

install(FILES ${PARFAIT_HEADERS}  ${MESSAGE_PASSER_HEADERS}
    DESTINATION parfait/include)

install(TARGETS MessagePasser 
    EXPORT parfait-targets
    DESTINATION parfait/lib)

install(FILES parfaitConfig.cmake 
    DESTINATION parfait)
