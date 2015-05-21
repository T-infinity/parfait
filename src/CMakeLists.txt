set(parfait_SRC
    "adt_3d_extent.cpp"                     
    "adt_3d_point.cpp"                      
    "motion_matrix.cpp"
    "byteswap.cpp"                          
    "cart_block.cpp"                        
    "parallel_mesh_reader_naive.cpp"
    "cart_mesh.cpp"                         
    "parallel_mesh_redistributor.cpp"
    "contiguous_matrix_multiplication.cpp"  
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
    "message_passer_serial.cpp"
    "vtk_volume_writer.cpp")

set(parfait_parallel_SRC
    "parallel_mesh_importer_distribute.cpp"
    "parallel_mesh_redistributor_ghost.cpp"
    "message_passer.cpp"                    
    )

set(parfait_links
    metis
    irc
    imf
    )

set(parallel_links
    ${MPI_LIBRARIES}
    parmetis
    )


if(mpi_installed AND parmetis_installed)
    set(parfait_SRC ${parfait_SRC} ${parfait_parallel_SRC})
    set(parfait_links ${parfait_links} ${parallel_links})
    message("Parfait configured for parallel")
endif()

add_library(parfait STATIC ${parfait_SRC})
target_link_libraries(parfait ${parfait_links})
