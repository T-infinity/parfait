#ifndef NODE_TO_NODE_H
#define NODE_TO_NODE_H

#include <stdio.h>
#include <algorithm>
#include "vector_tools.h"
#include "generic_mesh.h"
#include "message_passer.h"
#include <assert.h>

using std::vector;
using MessagePasser::Rank;

// There are two functions here:
// 1. buildUniqueNodeList()
//      -- takes any mesh
//
//      -- returns a list of unique node
//         ids that exist in the connectivity
//         on this partition
//
// 2. buildNodeToNode()
// 		-- takes any mesh
// 		-- and takes a sorted list of node ids
// 		   (in serial, this is just a vector of
// 		    integers from 0 to number of nodes)
//
// 		-- returns a vector of vectors
// 		   example:
// 		   // get n2n connectivity
// 		   vector<vector<int> > node2node buildNodeToNode(mesh,nodeIds);
// 		   // get first neighbor of node 9
// 		   int nbr = node2node[9][0];

// ----------------------------------------------------------------------
// NOTE: 
//       In serial, the unique node ids list is just a vector of integers
//       from 0-nnodes, and n2n is a full node to node connectivity
//
//       In parallel, the uniqueNodeList must be built because each
//       process does not own all of the nodes in its cell connectivities
//       yet.  The resulting n2n connectivity is a partial n2n on each
//       process.  Comminication will have to be done after the fact in
//       order to resolve the complete n2n connectivity.
template <typename T>
void putAllNodesIntoList(Mesh<T> &mesh, vector<int> &nodeIds);

template<typename T>
vector<int> buildUniqueNodeList(T &meshInterface)
{
	Mesh<T> mesh(meshInterface);
	int sizeEstimate = 6*mesh.numberOfCells();
	vector<int> nodeIds;
	nodeIds.reserve(sizeEstimate);
    putAllNodesIntoList(mesh, nodeIds);

    sort(nodeIds.begin(),nodeIds.end());
	// remove duplicates
	nodeIds.erase(unique(nodeIds.begin(),nodeIds.end()),nodeIds.end());
	return nodeIds;
}

template<typename T>
void putAllNodesIntoList(Mesh<T> &mesh, vector<int> &nodeIds) {
// put all nodes from all cells into list
    for(auto cell : mesh.cells()) {
		int nnodes = cell.numberOfNodes();
		vector<int> tmp = cell.getNodes();
		for(int i=0;i<nnodes;i++)
			nodeIds.push_back(tmp[i]);
		for(int i=0;i<nnodes;i++)
			nodeIds.push_back(tmp[i]);
	}
}

template<typename T>
std::vector<std::vector<int>> buildNodeToNode(T &meshInterface){
    std::vector<int> nodeIds(meshInterface.numberOfNodes());
    for(int i = 0; i < nodeIds.size(); i++){
        nodeIds[i] = i;
    }
    return buildNodeToNode(meshInterface, nodeIds);
}

template<typename T>
vector<vector<int> > buildNodeToNode(T &meshInterface,vector<int> &nodeIds) {
	Mesh<T> mesh(meshInterface);

	vector<vector<int> > n2n;
	n2n.resize((int)nodeIds.size());
	// populate n2n connectivity
	int percentDone = 0;
	int count = 0;
	for(auto cell:mesh.cells()) {
		for(auto face:cell) {
			int nvert = face.numberOfNodes();
			vector<int> tmp = face.getNodes();
			for(int i=0;i<nvert;i++) {
				int left = tmp[i];
				int right = tmp[(i+1)%nvert];
				int position  = (int)(std::lower_bound(nodeIds.begin(),nodeIds.end(),left)
					            - nodeIds.begin());
				assert(nodeIds[position] == left);
				insertUnique(n2n[position],right);
			}
		}
	}

	return n2n;
}

#endif
