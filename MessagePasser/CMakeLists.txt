cmake_minimum_required(VERSION 3.6)

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

add_definitions(
        -DPARFAIT_WITH_PARMETIS
)


file(GLOB_RECURSE PARFAIT_HEADERS
        ../parfait/*.h
        ../parfait/*.hpp
        )

file(GLOB_RECURSE MESSAGE_PASSER_HEADERS
        *.h
        *.hpp
        )

set(SOURCE_FILES
        src/SurfaceNeighbors.cpp
        )

set(PARFAIT_PARALLEL_TESTS_SRC
        test/AllTests.cpp
        test/AllGathervTests.cpp
        test/AllGatherTests.cpp
        test/GathervTests.cpp
        test/MessagePasserStatusTests.cpp
        test/AsyncSends_tests.cpp
        test/ParallelSumTests.cpp
        test/SendRecvScalarTests.cpp
        test/SendRecvVectorTests.cpp
        test/SendRecvVectorOfVectorsTests.cpp
        test/GatherTests.cpp
        test/BroadcastTests.cpp
        test/ScatterTests.cpp
        test/ScattervTests.cpp
        test/ReductionTests.cpp
        test/MessagePasserBasicsTests.cpp
        test/SelfSend_test.cpp
        test/StreamTests.cpp
        )

add_executable(MessagePasserTests
        ${PARFAIT_PARALLEL_TESTS_SRC}
        ${PARFAIT_HEADERS}
        ${MESSAGE_PASSER_HEADERS}
        )

target_include_directories(MessagePasserTests PUBLIC
        ../parfait/src/
        ../
        test
        ${PARMETIS_INCLUDE_DIRS}
        ${METIS_INCLUDE_DIRS}
        ${MPI_INCLUDE_PATH}
        ${JSON_INCLUDE_DIR}
        ${CATCH_INCLUDE_DIR}
)

add_dependencies(MessagePasserTests Catch)

target_link_libraries(MessagePasserTests
        ${MPI_LIBRARIES}
        ${PARMETIS_LIBRARY}
        ${METIS_LIBRARY}
        )

install(FILES ${MESSAGE_PASSER_HEADERS}
        DESTINATION parfait/include/MessagePasser)

install(FILES MessagePasserConfig.cmake
        DESTINATION parfait)
