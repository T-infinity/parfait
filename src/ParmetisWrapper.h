#ifndef PARMETISWRAPPER_H
#define PARMETISWRAPPER_H

#ifdef PARFAIT_WITH_PARMETIS
#include "parmetis.h"
#endif

// You need to provide the following:
// --Rank,nproc         
// --proc_node_map     = array of length nproc+1 
//                       (for 2 procs with 5 nodes
//                       each, this array would be:
//                       [0,5,10]
// --ia                = CRS map into ja
// --ja                = contains adjacency info
//
//
// For a single tet:
//
//          3 
//         /|\        Each node is connected to 3 nodes
//        / | \
//       /  |  \     
//      /   |   \
//     0----|----2
//		\   |   /
//       \  |  /
//        \ | /
//         \|/
//          1
//    ia = [0,3,6,9,12]
//    ja = [1,2,3, 0,2,3, 0,1,3, 0,1,2]
//
void PartitionMesh(int rank,int nproc,
		int *proc_node_map,
		int *ia,
		int *ja,
		int *part_vec);

#include "parmetis_wrapper.hpp"
#endif
