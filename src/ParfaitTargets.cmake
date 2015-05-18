cmake_minimum_required(VERSION 2.8)

PROJECT(parfait)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PARFAIT_CMAKE_DIR})

set(parfait_SRC 
    "${PARFAIT_CMAKE_DIR}/parallel_mesh_redistributor_ghost.cpp"
      "${PARFAIT_CMAKE_DIR}/message_passer.cpp"
      "${PARFAIT_CMAKE_DIR}/motion_matrix.cpp"
      "${PARFAIT_CMAKE_DIR}/imported_ugrid.cpp"
      "${PARFAIT_CMAKE_DIR}/cart_block.cpp"
      "${PARFAIT_CMAKE_DIR}/ugrid_reader.cpp"
      "${PARFAIT_CMAKE_DIR}/fun3d_mesh.cpp"
      "${PARFAIT_CMAKE_DIR}/mapbc_reader.cpp"
      "${PARFAIT_CMAKE_DIR}/adt_3d_extent.cpp"
      "${PARFAIT_CMAKE_DIR}/ugrid_writer.cpp"
      "${PARFAIT_CMAKE_DIR}/parallel_mesh_reader_naive.cpp"
      "${PARFAIT_CMAKE_DIR}/vtk_volume_writer.cpp"
      "${PARFAIT_CMAKE_DIR}/parallel_mesh_importer_distribute.cpp"
      "${PARFAIT_CMAKE_DIR}/contiguous_matrix_multiplication.cpp"
      "${PARFAIT_CMAKE_DIR}/parmetis_wrapper.cpp"
      "${PARFAIT_CMAKE_DIR}/vtk_extent_writer.cpp"
      "${PARFAIT_CMAKE_DIR}/adt_3d_point.cpp"
      "${PARFAIT_CMAKE_DIR}/cart_mesh.cpp"
      "${PARFAIT_CMAKE_DIR}/message_passer_serial.cpp"
      "${PARFAIT_CMAKE_DIR}/extent.cpp"
      "${PARFAIT_CMAKE_DIR}/vtk_hex_writer.cpp"
      "${PARFAIT_CMAKE_DIR}/kd_tree.cpp"
      "${PARFAIT_CMAKE_DIR}/byteswap.cpp"
      "${PARFAIT_CMAKE_DIR}/parallel_mesh_redistributor.cpp")

add_library(parfait STATIC ${parfait_SRC})
