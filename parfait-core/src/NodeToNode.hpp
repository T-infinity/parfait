#include "VectorTools.h"
#include "CGNS.h"
#include "MessagePasser.h"

template<typename MeshType>
std::vector <std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::   buildNodeToNodeConnectivity() {
    for (int cellId = 0; cellId < mesh.numberOfTets(); cellId++) {
        auto cell = mesh.getTet(cellId);
        for (int edge = 0; edge < CGNS::Tet::numberOfEdges(); edge++) {
            auto e = CGNS::Tet::edge_to_node[edge];
            addEdge(cell[e[0]], cell[e[1]]);
        }
    }
    for (int cellId = 0; cellId < mesh.numberOfPyramids(); cellId++) {
        auto cell = mesh.getPyramid(cellId);
        for (int edge = 0; edge < CGNS::Pyramid::numberOfEdges(); edge++) {
            auto e = CGNS::Pyramid::edge_to_node[edge];
            addEdge(cell[e[0]], cell[e[1]]);
        }
    }
    for (int cellId = 0; cellId < mesh.numberOfPrisms(); cellId++) {
        auto cell = mesh.getPrism(cellId);
        for (int edge = 0; edge < CGNS::Prism::numberOfEdges(); edge++) {
            auto e = CGNS::Prism::edge_to_node[edge];
            addEdge(cell[e[0]], cell[e[1]]);
        }
    }
    for (int cellId = 0; cellId < mesh.numberOfHexes(); cellId++) {
        auto cell = mesh.getHex(cellId);
        for (int edge = 0; edge < CGNS::Hex::numberOfEdges(); edge++) {
            auto e = CGNS::Hex::edge_to_node[edge];
            addEdge(cell[e[0]], cell[e[1]]);
        }
    }
    std::vector<std::vector<int>> n2n;
    for (auto& row:node_to_node)
        n2n.push_back(std::vector<int>(row.begin(), row.end()));
    return n2n;
}

template <typename MeshType>
void Parfait::NodeToNodeBuilder<MeshType>::addEdge(int left, int right) {
    node_to_node[right].insert(left);
    node_to_node[left].insert(right);
}