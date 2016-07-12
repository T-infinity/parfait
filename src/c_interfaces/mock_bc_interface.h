#pragma once


#ifdef __cplusplus
extern "C" {
#endif

void mock_bc_initialize(const char *filename,
                        int (*getFaceTag)(int),
                        void (*getFaceNodes)(int, int *),
                        int (*getComponentId)(int));

int mock_get_boundary_condition(int face_id);


#ifdef __cplusplus
}
#endif
