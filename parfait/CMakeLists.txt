cmake_minimum_required(VERSION 3.0)

PROJECT(parfait-core-unit-tests)
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

file(GLOB_RECURSE PARFAIT_HEADERS
        "*.h"
        "*.hpp"
        )


add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/grids/6cell.lb8.ugrid")
add_definitions(-DSIX_CELL_TEST_JSON="${CMAKE_SOURCE_DIR}/grids/6cell.json")
add_definitions(-DRAMP_TEST_JSON="${CMAKE_SOURCE_DIR}/grids/ramp.json")
add_definitions(-DCOMBINED_TEST_JSON="${CMAKE_SOURCE_DIR}/grids/combined.json")

set(SERIAL_TESTS_SOURCES
        test/AllTests.cpp
        test/Adt3DExtentTests.cpp
        test/Adt3DPointTests.cpp
        test/AdtTests.cpp
        test/AflrToCGNSTests.cpp
        test/BlockSparseMatrixTests.cpp
        test/BuildCRSTests.cpp
        test/CartBlockSliceTests.cpp
        test/CartBlockTests.cpp
        test/CGNSToAflrTests.cpp
        test/CGNSToFun3dTests.cpp
        test/CGNSToVtkTests.cpp
        test/DualMetricsTests.cpp
        test/ExtentMeshWrapperTests.cpp
        test/EdgeFieldLeastSquaresGradientTests.cpp
        test/ExtentBuilderTests.cpp
        test/ExtentTests.cpp
        test/FacetTests.cpp
        test/Fun3DToCGNSTests.cpp
        test/LinearPartitionerTests.cpp
        test/SparseMatrixTests.cpp
        test/MeshInterfaceTests.cpp
        test/ParfaitMeshTests.cpp
        test/PointTest.cpp
        test/StencilLeastSquaresGradientTests.cpp
        test/StringToolsTests.cpp
        test/STLTests.cpp
        test/VectorToolsTests.cpp
        test/GenericMeshToolsTests.cpp
        test/ImportedUgridTests.cpp
        test/NodeToCellTests.cpp
        test/EdgeBuilderTests.cpp
        test/HexMetricsTests.cpp
        test/PyramidMetricsTests.cpp
        test/PrismMetricsTests.cpp
        test/TetMetricsTests.cpp
        test/NodeToNodeBuilderTests.cpp
        test/ParallelNodeToNodeTests.cpp
        test/NodeBasedRedistributorTests.cpp
        test/PreProcessorTests.cpp
        test/ParallelMeshReaderTests.cpp
        test/DistanceCalculatorTests.cpp
        test/ParallelExtent_test.cpp
        test/GridSystemPreProcessorTests.cpp
        test/SyncFieldTests.cpp
        test/SyncPatternTests.cpp
        )


add_executable(ParfaitTests ${SERIAL_TESTS_SOURCES} ${PARFAIT_HEADERS})
target_include_directories(ParfaitTests PUBLIC
        ../
        ${MPI_INCLUDE_PATH}
        ${JSON_INCLUDE_DIR}
        ${CATCH_INCLUDE_DIR}
        ${PARMETIS_INCLUDE_DIRS}
        ${METIS_INCLUDE_DIRS}
        )
target_link_libraries(ParfaitTests
        ${MPI_LIBRARIES}
        ${PARMETIS_LIBRARY}
        ${METIS_LIBRARY}
        )
add_dependencies(ParfaitTests catch)


install(FILES ${PARFAIT_HEADERS}
        DESTINATION parfait/include/parfait)

install(FILES parfaitConfig.cmake
        DESTINATION parfait)
