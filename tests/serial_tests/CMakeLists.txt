
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitSerialTests)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.lb8.ugrid")

file(GLOB_RECURSE HEADERS
    "../src/*.h"
    "../src/*.hpp"
)

set(SERIAL_TESTS_SOURCES
        catch_main.cpp
        AflrToCGNSTests.cpp
        CGNSToAflrTests.cpp
        CGNSToFun3dTests.cpp
        CGNSToVtkTests.cpp
        edge_map/EdgeMapTests.cpp
        extent/ExtentTests.cpp
        VtkToCGNSTests.cpp
        Fun3DToCGNSTests.cpp
        parfait_mesh/ParfaitMesh_test.cpp
        connectivity/NodeToNodeTests.cpp
        CGNS_Ordering/CGNSEdgeTests.cpp
        cart_block/CartBlockTests.cpp
        cart_block/CartBlockSliceTests.cpp
        point/PointTest.cpp
        vectors/VectorToolsTests.cpp
        LinearPartitioner/LinearPartitionerTests.cpp
        vectors/LegacyArrayTests.cpp
)

set(LEGACY_SERIAL_TESTS_SOURCES
        AllTests.cpp  
        vtk_slice_writer_tests.cpp
        extent/ExtentBuilderTests.cpp
        VtkFieldTests/VtkFieldTests.cpp
        VtkGenericTests/VtkGenericTests.cpp
        VtkSurfaceTests/VtkSurfaceTests.cpp
        adt/adt_3d_extent_test.cpp
        adt/adt_3d_point_test.cpp
        adt/adt_test.cpp
        build_neighbors/build_neighbors_test.cpp
        cart_mesh/cart_mesh_test.cpp
        generic_mesh_tools/GenericMeshToolsTests.cpp
        #Geometry/stl_test.cpp
        #Geometry/facet_test.cpp
        #Geometry/STLReader_test.cpp
        high_level_mesh_searching/AdtBuilderTests.cpp
        mesh_interfaces/fun3d_mesh_tests.cpp
        mesh_interfaces/imported_ugrid_tests.cpp
        range_loop/RangeLoopTests.cpp
        ugridWriterTests/ugridWriterTests.cpp
        ugrid_factory/ugrid_factory_tests.cpp
        VtkExplodedViewWriter/VtkExplodedViewWriter_test.cpp
       )

add_executable(LegacySerialTests ${LEGACY_SERIAL_TESTS_SOURCES} ${HEADERS})
include_directories(../../tinyxml/src ../)
target_link_libraries(LegacySerialTests
        CppUTest
        tinyxml)

add_executable(ParfaitSerialTests ${SERIAL_TESTS_SOURCES} ${HEADERS} ../catch.hpp)
