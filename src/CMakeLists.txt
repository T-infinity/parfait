cmake_minimum_required(VERSION 2.8)

PROJECT(parfait_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(parfait_lib_SRC "parallel_mesh_redistributor_ghost.cpp"
      "message_passer.cpp"
      "motion_matrix.cpp"
      "imported_ugrid.cpp"
      "cart_block.cpp"
      "ugrid_reader.cpp"
      "fun3d_mesh.cpp"
      "mapbc_reader.cpp"
      "adt_3d_extent.cpp"
      "ugrid_writer.cpp"
      "parallel_mesh_reader_naive.cpp"
      "vtk_volume_writer.cpp"
      "parallel_mesh_importer_distribute.cpp"
      "contiguous_matrix_multiplication.cpp"
      "parmetis_wrapper.cpp"
      "vtk_extent_writer.cpp"
      "adt_3d_point.cpp"
      "cart_mesh.cpp"
      "message_passer_serial.cpp"
      "extent.cpp"
      "vtk_hex_writer.cpp"
      "kd_tree.cpp"
      "byteswap.cpp"
      "parallel_mesh_redistributor.cpp")


add_library(parfait_lib STATIC ${parfait_lib_SRC})

