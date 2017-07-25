cmake_minimum_required(VERSION 3.0)

PROJECT(parfait-core-unit-tests)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")
set (CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE PARFAIT_CORE_HEADERS
    "src/*.h"
    "src/*.hpp"
)


set(SERIAL_TESTS_SOURCES
  test/AllTests.cpp
  test/Adt3DExtentTests.cpp
  test/Adt3DPointTests.cpp
  test/AdtBuilderTests.cpp
  test/AdtTests.cpp
  test/AflrToCGNSTests.cpp
  test/BuildNeighborsTests.cpp
  test/BuildCRSTests.cpp
  test/CartBlockSliceTests.cpp
  test/CartBlockTests.cpp
  test/CGNSEdgeTests.cpp
  test/CGNSToAflrTests.cpp
  test/CGNSToFun3dTests.cpp
  test/CGNSToVtkTests.cpp
  test/CompressedMapTests.cpp
  test/ExtentMeshWrapperTests.cpp
  test/EdgeMapTests.cpp
        test/EdgeFieldLeastSquaresGradientTests.cpp
        test/ExtentBuilderTests.cpp
  test/ExtentTests.cpp
  test/FacetTests.cpp
  test/Fun3DToCGNSTests.cpp
  test/LegacyArrayTests.cpp
  test/LinearPartitionerTests.cpp
  test/MatrixTests.cpp
  test/MeshInterfaceTests.cpp
  test/NodeToNodeTests.cpp
  test/ParfaitMeshTests.cpp
  test/PointTest.cpp
  test/RangeLoopTests.cpp
        test/StencilLeastSquaresGradientTests.cpp
  test/StringToolsTests.cpp
  test/StreamTests.cpp
  test/STLTests.cpp
  test/STLReaderTests.cpp
  test/VectorToolsTests.cpp
  test/GenericMeshToolsTests.cpp
  test/ImportedUgridTests.cpp
  test/UgridFactoryTests.cpp
  test/UgridWriterTests.cpp
  test/NodeToCellTests.cpp
        test/EdgeBuilderTests.cpp
        )


add_executable(ParfaitSerialTests ${SERIAL_TESTS_SOURCES} ${PARFAIT_CORE_HEADERS})
target_include_directories(ParfaitSerialTests PUBLIC
    src 
    ${CATCH_INCLUDE_DIR}
    )
add_dependencies(ParfaitSerialTests catch)
