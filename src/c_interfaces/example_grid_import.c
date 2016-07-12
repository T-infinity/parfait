#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "parfait_interface.h"
#include "parfait_vis_interface.h"
#include "yoga_interface.h"
#include "mock_bc_interface.h"


int main(int argc, char *argv[]){
    MPI_Init(&argc,&argv);
    parfait_initialize("wing_store.xml");
    mock_bc_initialize("wing_store.xml",
                       &parfait_get_boundary_tag,
                       &parfait_get_nodes_in_face,
                       &parfait_get_component_grid_id);

    yoga_register(&parfait_get_number_of_nodes, &parfait_get_number_of_cells,
                  &parfait_get_number_of_nodes_in_cell,
                  &parfait_get_nodes_in_cell,
                  &parfait_get_global_node_id,
                  &parfait_get_component_grid_id, &parfait_get_node_xyz, &parfait_get_node_ownership,
                  &parfait_get_number_of_faces, &parfait_get_number_of_nodes_in_face,
                  &parfait_get_nodes_in_face, &mock_get_boundary_condition);


    vtk_register(&parfait_get_number_of_nodes,&parfait_get_number_of_cells,&parfait_get_number_of_nodes_in_cell,
           &parfait_get_nodes_in_cell,&parfait_get_node_xyz);


    parfait_finalize();

    MPI_Finalize();
}