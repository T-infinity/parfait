#ifndef NODE_TO_NODE_H
#define NODE_TO_NODE_H

#include <stdio.h>
#include <algorithm>
#include "VectorTools.h"
#include "GenericMesh.h"
#include "MessagePasser.h"
#include <assert.h>
#include <stdexcept>

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
namespace Parfait {
    template<typename MeshType>
    std::vector<int> putAllNodesIntoList(MeshType &mesh);

    template<typename T>
    std::vector<int> buildUniqueNodeList(T &meshInterface) {
        if(MessagePasser::Rank() == 0)
            printf("Building unique node list:\n");
        Mesh<T> mesh(meshInterface);
        if(MessagePasser::Rank() == 0)
            printf("Putting candidate nodes into list:\n");
        auto node_ids = putAllNodesIntoList(mesh);
        for(auto id:node_ids)
            if(id < 0)
                throw std::logic_error("Negative node id....");

        if(MessagePasser::Rank() == 0)
            printf("sorting:\n");
        sort(node_ids.begin(), node_ids.end());

        if(MessagePasser::Rank() == 0)
            printf("removing duplicates:\n");
        node_ids.erase(unique(node_ids.begin(), node_ids.end()), node_ids.end());
        if(MessagePasser::Rank() == 0)
            printf("Done removing duplicates:\n");
        printf("Rank %i: node_ids.size() %i\n",MessagePasser::Rank(), node_ids.size());
        return node_ids;
    }

    template<typename MeshType>
    std::vector<int> putAllNodesIntoList(MeshType &mesh) {
        std::vector<int> node_ids;
        for (int i=0;i<mesh.numberOfCells();i++) {
            std::vector<int> tmp = mesh.getNodesInCell(i);
            for (int id:tmp) {
                if(id < 0){
                    printf("Cell size %i: (",tmp.size());
                    for(auto xx:tmp){
                        printf("%i ",xx);
                    }
                    printf(")\n");
                    throw std::logic_error("Cell had negative ids");
                }
                node_ids.push_back(id);
            }
        }
        return node_ids;
    }

    template<typename T>
    std::vector<std::vector<int>> buildNodeToNode(T &meshInterface) {
        std::vector<int> nodeIds(meshInterface.numberOfNodes());
        for (int i = 0; i < nodeIds.size(); i++) {
            nodeIds[i] = i;
        }
        return buildNodeToNode(meshInterface, nodeIds);
    }

    template<typename T>
    std::vector<std::vector<int> > buildNodeToNode(T &meshInterface, std::vector<int> &nodeIds) {
        Mesh<T> mesh(meshInterface);

        std::vector<std::vector<int> > n2n;
        n2n.resize((int) nodeIds.size());
        if(MessagePasser::Rank() == 0)
            printf("Building local n2n connectivity:\n");
        for (int cell_id=0;cell_id<mesh.numberOfCells();cell_id++) {
            for (int face_id=0;face_id<mesh.numberOfFacesInCell(cell_id);face_id++) {
                int nvert = mesh.numberOfNodesInCellFace(cell_id,face_id);
                auto face = meshInterface.getNodesInCellFace(cell_id,face_id);

                for (int i = 0; i < nvert; i++) {
                    int left = face[i];
                    int right = face[(i + 1) % nvert];
                    int position = (int) (std::lower_bound(nodeIds.begin(), nodeIds.end(), left)
                                          - nodeIds.begin());
                    assert(nodeIds[position] == left);
                    insertUnique(n2n[position], right);
                }
            }
        }
        if(MessagePasser::Rank() == 0)
            printf("done with local conn\n");
        return n2n;
    }
}
#endif
