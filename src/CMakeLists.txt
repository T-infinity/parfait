set(parfait_SRC
    "adt_3d_extent.cpp"
    "adt.h"
    "adt.hpp"
    "adt_builder.h"
    "adt_elem.h"
    "adt_elem.hpp"
    "build_neighbors.h"
    "adt_3d_point.cpp"                      
    "motion_matrix.cpp"
    "byteswap.cpp"                          
    "cart_block.cpp"                        
    "parallel_mesh_reader_naive.cpp"
    "cart_mesh.cpp"
    "cell_to_cell.h"
    "checkpoint.h"
    "parallel_mesh_redistributor.cpp"
    "contiguous_matrix_multiplication.cpp"  
    "extent.cpp"
    "edge_map.h"
    "extent_builder.h"
    "parmetis_wrapper.cpp"
    "fun3d_mesh.cpp"
    "generic_cell.h"
    "generic_cell.hpp"
    "generic_face.h"
    "generic_face.hpp"
    "generic_mesh.h"
    "generic_mesh.hpp"
    "generic_mesh_acceptance_checks.h"
    "generic_mesh_tools.h"
    "generic_node.h"
    "generic_node.hpp"
    "generic_surface_mesh.h"
    "message_passer_broadcasts.hpp"
    "message_passer_gathers.hpp"
    "message_passer_recvs.hpp"
    "message_passer_reductions.hpp"
    "message_passer_scatters.hpp"
    "message_passer_sends.hpp"
    "message_passer_status.h"
    "message_passer_wait.hpp"
    "ugrid_reader.cpp"
    "node_to_cell.h"
    "node_to_node.h"
    "parmetis_prepper.h"
    "parmetis_prepper.hpp"
    "point.h"
    "point.hpp"
    "range_loop.h"
    "unit_transformer.h"
    "vector_tools.h"
    "vtk_field.h"
    "vtk_field.hpp"
    "vtk_generic_writer.h"
    "vtk_generic_writer.hpp"
    "vtk_field.hpp"
    "vtk_field.h"
    "vtk_slice_writer.h"
    "vtk_slice_writer.hpp"
    "vtk_surface_writer.h"
    "vtk_surface_writer.hpp"
    "vtk_slice_writer.h"
    "vtk_slice_writer.hpp"
    "vtk_volume_writer.hpp"
    "hex.h"
    "imported_ugrid_factory.h"
    "legacy_array.h"
    "mesh_mover.h"
    "mesh_slice.h"
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
    #irc
    #imf
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
