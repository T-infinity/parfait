cmake_minimum_required(VERSION 2.8)

PROJECT(CreateVolumes)
include ( FindPackageHandleStandardArgs )
set (CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)
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
  -DPARFAIT_WITH_MPI
  -DPARFAIT_WITH_PARMETIS
)

add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/../grids/6cell.lb8.ugrid")
add_definitions(-DSIX_CELL_TEST_XML="${CMAKE_SOURCE_DIR}/../grids/6cell.xml")


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../third-party-deps/tinyxml" 
    "${CMAKE_CURRENT_BINARY_DIR}/tinyxml"
)

set(SHARED_INCLUDE_DIRECTORIES
    ../parfait-core/src/
    ../parfait-parallel/src/
    ../third-party-deps/tinyxml/src
    src
)

file(GLOB_RECURSE HEADERS
    ../parfait-core/src/*.h
    ../parfait-core/src/*.hpp
    ../parfait-parallel/src/*.h
    ../parfait-parallel/src/*.hpp
    src/*.h
    src/*.hpp
)

set(MESSAGE_PASSER_SOURCES
    ../parfait-parallel/src/MessagePasser.cpp
)

set(VOLUMES_SOURCES
    src/main.cpp
)

set(TEST_SOURCES
    test/AllTests.cpp
    test/DualMetricsTests.cpp
    test/TetMetricsTests.cpp
    test/PrismMetricsTests.cpp
    test/PyramidMetricsTests.cpp
    test/HexMetricsTests.cpp
)

add_executable(UnitTests
    ${TEST_SOURCES}
)

target_include_directories(UnitTests
    PUBLIC
    test
    ../third-party-deps/catch/
    ${SHARED_INCLUDE_DIRECTORIES}
)

add_executable(CreateVolumesDemo
    ${VOLUMES_SOURCES}
    ${HEADERS}
    ${MESSAGE_PASSER_SOURCES}
)

target_include_directories(CreateVolumesDemo
    PUBLIC
    ${MPI_INCLUDE_PATH}
    ${SHARED_INCLUDE_DIRECTORIES}
)

target_link_libraries(CreateVolumesDemo
    ${MPI_LIBRARIES}
    tinyxml
    ${PARMETIS_LIBRARY}
    ${METIS_LIBRARY}
)
