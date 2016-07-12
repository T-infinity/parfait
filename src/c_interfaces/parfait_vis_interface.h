#pragma once

#ifdef __cplusplus
extern "C" {
#endif


void vtk_register(int (*getNumberOfNodes)(),
                                 int (*getNumberOfCells)(),
                                 int (*getNumberOfNodesInCell)(int),
                                 void (*getNodesInCell)(int,int*),
                                 void (*getPoint)(int,double*));



void vtk_create_volume_visualization(const char* name);

void vtk_cleanup();
#ifdef __cplusplus
}
#endif

