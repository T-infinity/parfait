#ifndef PARFAIT_PARFAIT_PREPROCESSOR_H
#define PARFAIT_PARFAIT_PREPROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

void parfait_create_grid_system(const char* filename);

int parfait_preprocessor_number_of_local_nodes();
int parfait_preprocessor_number_of_ghost_nodes();
int parfait_preprocessor_number_of_triangles();
int parfait_preprocessor_number_of_quads();
int parfait_preprocessor_number_of_tets();
int parfait_preprocessor_number_of_pyramids();
int parfait_preprocessor_number_of_prisms();
int parfait_preprocessor_number_of_hexs();

void parfait_preprocessor_fill_x(double* x);
void parfait_preprocessor_fill_y(double* y);
void parfait_preprocessor_fill_z(double* z);
void parfait_preprocessor_fill_global_node_ids(int *ids);
void parfait_preprocessor_fill_associated_grid_ids(int *ids);

void parfait_preprocessor_fill_triangles(int *triangles);
void parfait_preprocessor_fill_triangle_tags(int *tags);
void parfait_preprocessor_fill_triangle_bcs(int *bcs);
void parfait_preprocessor_fill_quads(int *quads);
void parfait_preprocessor_fill_quad_tags(int *quad_tags);
void parfait_preprocessor_fill_quad_bcs(int *quad_bcs);

void parfait_preprocessor_fill_tets(int* tets);
void parfait_preprocessor_fill_pyramids(int* pyramids);
void parfait_preprocessor_fill_prisms(int* prisms);
void parfait_preprocessor_fill_hexs(int* hexs);

void parfait_cleanup();

#ifdef __cplusplus
};
#endif

#endif //PARFAIT_PARFAIT_PREPROCESSOR_H
