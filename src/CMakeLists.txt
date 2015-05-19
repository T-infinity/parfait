set(parfait_SRC
    "adt_3d_extent.cpp"                     
    "message_passer_serial.cpp"
    "adt_3d_point.cpp"                      
    "motion_matrix.cpp"
    "byteswap.cpp"                          
    "parallel_mesh_importer_distribute.cpp"
    "cart_block.cpp"                        
    "parallel_mesh_reader_naive.cpp"
    "cart_mesh.cpp"                         
    "parallel_mesh_redistributor.cpp"
    "contiguous_matrix_multiplication.cpp"  
    "parallel_mesh_redistributor_ghost.cpp"
    "extent.cpp"                            
    "parmetis_wrapper.cpp"
    "fun3d_mesh.cpp"                        
    "ugrid_reader.cpp"
    "imported_ugrid.cpp"                    
    "ugrid_writer.cpp"
    "kd_tree.cpp"                           
    "vtk_extent_writer.cpp"
    "mapbc_reader.cpp"                      
    "vtk_hex_writer.cpp"
    "message_passer.cpp"                    
    "vtk_volume_writer.cpp")

add_library(parfait STATIC ${parfait_SRC})
target_link_libraries(parfait parmetis metis irc imf)
