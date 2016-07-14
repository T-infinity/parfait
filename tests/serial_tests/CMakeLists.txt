cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitSerialTests)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

file(GLOB_RECURSE HEADERS
    "../src/*.h"
    "../src/*.hpp"
)

set(SERIAL_TESTS_SOURCES
        catch_main.cpp
        Adt3DExtentTests.cpp
        Adt3DPointTests.cpp
        AdtBuilderTests.cpp
        AdtTests.cpp
        AflrToCGNSTests.cpp
        BuildNeighborsTests.cpp
        CartBlockSliceTests.cpp
        CartBlockTests.cpp
        CGNSEdgeTests.cpp
        CGNSToAflrTests.cpp
        CGNSToFun3dTests.cpp
        CGNSToVtkTests.cpp
        CompressedMapTests.cpp
        ExtentMeshWrapperTests.cpp
        EdgeMapTests.cpp
        ExtentBuilderTests.cpp
        ExtentTests.cpp
        FacetTests.cpp
        Fun3DToCGNSTests.cpp
        LegacyArrayTests.cpp
        LinearPartitionerTests.cpp
        MatrixTests.cpp
        NodeToNodeTests.cpp
        ParfaitMeshTests.cpp
        PointTest.cpp
        RangeLoopTests.cpp
        StreamTests.cpp
        STLTests.cpp
        STLReaderTests.cpp
        VectorToolsTests.cpp
        VtkExplodedViewWriterTests.cpp
        VtkFieldTests.cpp
        VtkSliceWriterTests.cpp
        VtkToCGNSTests.cpp
        GenericMeshToolsTests.cpp
        ImportedUgridTests.cpp
        UgridFactoryTests.cpp
        UgridWriterTests.cpp
        VtkGenericTests.cpp
        VtkSurfaceTests.cpp
        )

include_directories(../)

add_executable(ParfaitSerialTests ${SERIAL_TESTS_SOURCES} ${HEADERS} ../catch.hpp)
