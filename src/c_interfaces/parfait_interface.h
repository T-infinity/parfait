#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void parfait_initialize(const char*);
int parfait_get_number_of_nodes();
void parfait_get_node_xyz(int,double*);
long parfait_get_global_node_id(int);
int parfait_get_node_ownership(int);
int parfait_get_component_grid_id(int);
int parfait_get_number_of_cells();
int parfait_get_number_of_nodes_in_cell(int);
void parfait_get_nodes_in_cell(int,int*);
int parfait_get_number_of_faces();
int parfait_get_number_of_nodes_in_face(int);
int parfait_get_boundary_tag(int);
void parfait_get_nodes_in_face(int,int*);
void parfait_finalize();

#ifdef __cplusplus
}
#endif

