#ifndef EDGE_MAP_H
#define EDGE_MAP_H

#include <array>
#include <vector>
namespace Parfait {
    template<class MeshType>
    class EdgeMap {

    private:
        MeshType &mesh;

    public:
        typedef std::array<int, 2> Edge;

        EdgeMap(MeshType &mesh);
        int getOtherNode(const Edge &e, int node1);
        int getEdgeId(const Edge &e) const;

        std::vector<Edge> edges;
        std::vector<std::vector<int>> edgeToCell;
        std::vector<std::vector<int>> nodeToEdge;

    private:
        std::vector<std::vector<int>> buildNodeToEdge();
    };
}

#include "GenericMeshTools.h"
#include "CellToCell.h"
namespace Parfait{
    template<typename MeshType>
    EdgeMap<MeshType>::EdgeMap(MeshType &mesh_in)
            : mesh(mesh_in), edges(GenericMeshTools::getUniqueEdges(mesh)) {
        edgeToCell = buildEdgeToCell(mesh, edges);
        nodeToEdge = buildNodeToEdge();
    }

    template<typename MeshType>
    int EdgeMap<MeshType>::getOtherNode(const Edge &e, int node1) {
        if (e[0] == node1) {
            return e[1];
        }
        else {
            return e[0];
        }
    }

    template<typename MeshType>
    std::vector<std::vector<int>> EdgeMap<MeshType>::buildNodeToEdge() {

        std::vector<std::vector<int>> nodeToEdge(mesh.numberOfNodes());
        for (int edgeId = 0; edgeId < (int)edges.size(); edgeId++) {
            auto &edge = edges[edgeId];
            insertUnique(nodeToEdge[edge[0]], edgeId);
            insertUnique(nodeToEdge[edge[1]], edgeId);
        }
        return nodeToEdge;
    }

    template<typename MeshType>
    int EdgeMap<MeshType>::getEdgeId(const Edge &e) const {
        auto candidates0 = nodeToEdge[e[0]];
        auto candidates1 = nodeToEdge[e[1]];

        for (int edge1 : candidates0) {
            for (int edge2 : candidates1) {
                if (edge1 == edge2) {
                    return edge1;
                }
            }
        }

        throw std::logic_error("Nodes Do not share an edge.");
    }
}
#endif
