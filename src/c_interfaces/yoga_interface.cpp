#include <stdio.h>
#include <array>
#include "yoga_interface.h"

void yoga_register(int (*numberOfNodes)(),
                                 int (*numberOfCells)(),
                                 int (*numberOfNodesInCell)(int),
                                 void (*getNodesInCell)(int,int*),
                                 long (*getGlobalNodeId)(int),
                                 int (*getComponentId)(int),
                                 void (*getPoint)(int,double*),
                                 int (*isGhostNode)(int),
                                 int (*getNumberOfBoundaryFaces)(),
                                 int (*numberOfNodesInBoundaryFace)(int),
                                 void (*getNodesInFace)(int,int*),
                                 int (*getBoundaryCondition)(int)){
    printf("Yoga, registering functions from solver\n");
    printf("Mesh data\n");
    printf("------------------------------------------\n");
    printf("# nodes: %i\n",numberOfNodes());
    printf("# cells: %i\n",numberOfCells());
}

void yoga_perform_domain_assembly(){
  printf("Yoga: performing assembly\n");
}

int yoga_get_node_status(int node_id){
  return 0;
}

int yoga_number_of_donors_for_node(int node_id){
  return 4;
}

void yoga_get_donor_ids_for_node(int node_id, long* ids){
  ids[0] = 99;
  ids[1] = 98;
  ids[2] = 97;
  ids[3] = 96;
}

void yoga_get_donor_weights_for_node(int node_id, double* weights){
  weights[0] = 0.0;
  weights[1] = 1.0;
  weights[2] = 2.0;
  weights[3] = 3.0;
}

void yoga_cleanup(){
  printf("Yoga: cleanup\n");
}
