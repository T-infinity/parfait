
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitSerialTests)
add_definitions(-DSIX_CELL_TEST_GRID="${CMAKE_SOURCE_DIR}/tests/grids/6cell.ugrid")

set(PARFAIT_SERIAL_TESTS_SRC
        AllTests.cpp  
        vtk_slice_writer_tests.cpp
        extent/ExtentBuilderTests.cpp
        extent/extent_test.cpp
        VtkFieldTests/VtkFieldTests.cpp
        VtkGenericTests/VtkGenericTests.cpp
        VtkSurfaceTests/VtkSurfaceTests.cpp
        adt/adt_3d_extent_test.cpp
        adt/adt_3d_point_test.cpp
        adt/adt_test.cpp
        build_neighbors/build_neighbors_test.cpp
        cart_block/CartBlockTests.cpp
        cart_mesh/cart_mesh_test.cpp
        connectivity/NodeToNodeTests.cpp
        edge_map/edge_map_test.cpp
        generic_mesh_tools/GenericMeshToolsTests.cpp
        Geometry/stl_test.cpp
        Geometry/facet_test.cpp
        high_level_mesh_searching/AdtBuilderTests.cpp
        mesh_interfaces/fun3d_mesh_tests.cpp
        mesh_interfaces/imported_ugrid_tests.cpp
        point/point_test.cpp
        range_loop/RangeLoopTests.cpp
        ugridWriterTests/ugridWriterTests.cpp
        ugrid_factory/ugrid_factory_tests.cpp
        vectors/LegacyArrayTests.cpp
        vectors/vector_tools_tests.cpp
        VtkExplodedViewWriter/VtkExplodedViewWriter_test.cpp
       )


add_executable(ParfaitSerialTests ${PARFAIT_SERIAL_TESTS_SRC})
target_link_libraries(ParfaitSerialTests CppUTest
        )
