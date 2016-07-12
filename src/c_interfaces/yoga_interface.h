#pragma once

#ifdef __cplusplus
extern "C" {
#endif


void yoga_register(int (*getNumberOfNodes)(),
                                 int (*getNumberOfCells)(),
                                 int (*getNumberOfNodesInCell)(int),
                                 void (*getNodesInCell)(int,int*),
                                 long (*getGlobalNodeId)(int),
                                 int (*getComponentId)(int),
                                 void (*getPoint)(int,double*),
                                 int (*isGhostNode)(int),
                                 int (*getNumberOfBoundaryFaces)(),
                                 int (*numberOfNodesInBoundaryFace)(int),
                                 void (*getNodesInFace)(int,int*),
                                 int (*getBoundaryCondition)(int));

void yoga_perform_domain_assembly();


int yoga_get_node_status(int node_id);
void yoga_get_donor_ids_for_node(int node_id, long* ids);
int yoga_number_of_donors_for_node(int node_id);
void yoga_get_donor_weights_for_node(int node_id, double* weights);

void yoga_cleanup();
#ifdef __cplusplus
}
#endif

