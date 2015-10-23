
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitSerialTests)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

file(GLOB_RECURSE HEADERS
    "../src/*.h"
    "../src/*.hpp"
)

set(SERIAL_TESTS_SOURCES
        catch_main.cpp
        adt/Adt3DExtentTests.cpp
        adt/Adt3DPointTests.cpp
        adt/AdtTests.cpp
        AflrToCGNSTests.cpp
        build_neighbors/build_neighbors_test.cpp
        cart_block/CartBlockSliceTests.cpp
        cart_block/CartBlockTests.cpp
        CGNS_Ordering/CGNSEdgeTests.cpp
        CGNSToAflrTests.cpp
        CGNSToFun3dTests.cpp
        CGNSToVtkTests.cpp
        CompressedMapTests.cpp
        connectivity/NodeToNodeTests.cpp
        edge_map/EdgeMapTests.cpp
        extent/ExtentBuilderTests.cpp
        extent/ExtentTests.cpp
        Fun3DToCGNSTests.cpp
        high_level_mesh_searching/AdtBuilderTests.cpp
        LinearPartitioner/LinearPartitionerTests.cpp
        MatrixTests.cpp
        parfait_mesh/ParfaitMesh_test.cpp
        point/PointTest.cpp
        range_loop/RangeLoopTests.cpp
        StreamTests.cpp
        vectors/LegacyArrayTests.cpp
        vectors/VectorToolsTests.cpp
        VtkExplodedViewWriter/VtkExplodedViewWriterTests.cpp
        VtkFieldTests/VtkFieldTests.cpp
        VtkSliceWriterTests.cpp
        VtkToCGNSTests.cpp
        generic_mesh_tools/GenericMeshToolsTests.cpp
        MeshConnectivityTests.cpp
        MeshMetaDataTests.cpp)

set(LEGACY_SERIAL_TESTS_SOURCES
        AllTests.cpp  
        VtkGenericTests/VtkGenericTests.cpp
        VtkSurfaceTests/VtkSurfaceTests.cpp
        generic_mesh_tools/GenericMeshToolsTests.cpp
        #Geometry/stl_test.cpp
        #Geometry/facet_test.cpp
        #Geometry/STLReader_test.cpp
        mesh_interfaces/imported_ugrid_tests.cpp
        ugridWriterTests/ugridWriterTests.cpp
        ugrid_factory/ugrid_factory_tests.cpp
       )

add_executable(LegacySerialTests ${LEGACY_SERIAL_TESTS_SOURCES} ${HEADERS})
include_directories(../../tinyxml/src ../)
target_link_libraries(LegacySerialTests
        CppUTest
        tinyxml)

add_executable(ParfaitSerialTests ${SERIAL_TESTS_SOURCES} ${HEADERS} ../catch.hpp)
