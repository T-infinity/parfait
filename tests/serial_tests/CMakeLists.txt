
cmake_minimum_required(VERSION 2.8)

PROJECT(ParfaitSerialTests)
set(CATCH_DIR "/ump/fldmd/home/cdruyorj/cdruyor_local/catch/include")
add_definitions(-DSIX_CELL_TEST_GRID="${parfait_DIR}/tests/grids/6cell.ugrid")
find_package(parfait REQUIRED)

include_directories(
    "${CATCH_DIR}"
    "${parfait_DIR}/src"
    )

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
        connectivity/CellToCellTests.cpp  
        connectivity/NodeToNodeTests.cpp

       )


add_executable(ParfaitSerialTests ${PARFAIT_SERIAL_TESTS_SRC})
target_link_libraries(ParfaitSerialTests parfait CppUTest)
