cmake_minimum_required(VERSION 3.6)

PROJECT(parfait-core-unit-tests)
set (CMAKE_CXX_STANDARD 14)


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
        test/CGNSFaceExtractionTests.cpp
        test/CGNSToAflrTests.cpp
        test/CGNSToFun3dTests.cpp
        test/CGNSToVtkTests.cpp
        test/DualMetricsTests.cpp
        test/ExtentMeshWrapperTests.cpp
        test/ExtentBuilderTests.cpp
        test/EdgesTests.cpp
        test/EdgeFieldLeastSquaresGradientTests.cpp
        test/ExtentTests.cpp
        test/FacetTests.cpp
        test/Fun3DToCGNSTests.cpp
        test/LinearPartitionerTests.cpp
        test/SparseMatrixTests.cpp
        test/MeshInterfaceTests.cpp
        test/NormalTests.cpp
        test/ParfaitMeshTests.cpp
        test/TreeDistTests.cpp
        test/PointTest.cpp
        test/StencilLeastSquaresGradientTests.cpp
        test/StringToolsTests.cpp
        test/STLTests.cpp
        test/VectorToolsTests.cpp
        test/MinimumStorageSetTests.cpp
        test/ImportedUgridTests.cpp
        test/NodeToCellTests.cpp
        test/HexMetricsTests.cpp
        test/GraphOrderingsTests.cpp
        test/PyramidMetricsTests.cpp
        test/PrismMetricsTests.cpp
        test/TetMetricsTests.cpp
        test/NodeToNodeBuilderTests.cpp
        test/ParallelNodeToNodeTests.cpp
        test/NodeBasedRedistributorTests.cpp
        test/PreProcessorTests.cpp
        test/ParallelMeshReaderTests.cpp
        test/TreeDistTests.cpp
        test/DistanceCalculatorTests.cpp
        test/ParallelExtent_test.cpp
        test/GridSystemPreProcessorTests.cpp
        test/SyncFieldTests.cpp
        test/SyncPatternTests.cpp
        )

# create the cmake-3.0 target
add_library(parfait INTERFACE)
target_include_directories(parfait INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:parfait/include>
        ${MPI_INCLUDE_PATH} ${METIS_INCLUDE_DIRS} ${PARMETIS_INCLUDE_DIRS})

target_link_libraries(parfait INTERFACE ${MPI_LIBRARIES} ${METIS_LIBRARY} ${PARMETIS_LIBRARY}
        MessagePasser::MessagePasser tracer::tracer)

add_executable(ParfaitTests ${SERIAL_TESTS_SOURCES} ${PARFAIT_HEADERS})
target_include_directories(ParfaitTests PUBLIC ${CATCH_INCLUDE_DIR} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:/.>
        )
target_link_libraries(ParfaitTests parfait nlohmann_json MessagePasser::MessagePasser)

add_dependencies(ParfaitTests Catch)


install(TARGETS parfait
        EXPORT parfait
        LIBRARY DESTINATION parfait/lib)

install(FILES ${PARFAIT_HEADERS} DESTINATION parfait/include/parfait)

install(EXPORT parfait
        NAMESPACE parfait:: DESTINATION parfait)

install(FILES parfaitConfig.cmake
        DESTINATION parfait)

