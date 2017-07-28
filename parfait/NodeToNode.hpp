#include "VectorTools.h"
#include "CGNSElements.h"
#include "EdgeBuilder.h"
template<typename MeshType>
std::vector <std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::buildNodeToNodeConnectivity() {
    EdgeBuilder edgeBuilder;
    std::vector<int> cell;
    for(int i=0;i<mesh.numberOfCells();i++){
        int n = mesh.numberOfNodesInCell(i);
        cell.resize(n);
        mesh.getNodesInCell(i,cell.data());
        switch(n){
            case 4:
                edgeBuilder.addCell(cell.data(),CGNS::Tet::edge_to_node);
                break;
            case 5:
                edgeBuilder.addCell(cell.data(),CGNS::Pyramid::edge_to_node);
                break;
            case 6:
                edgeBuilder.addCell(cell.data(),CGNS::Prism::edge_to_node);
                break;
            case 8:
                edgeBuilder.addCell(cell.data(),CGNS::Hex::edge_to_node);
                break;
            default:
                throw std::logic_error("NodeToNodeBuilder: only linear volume elements supported");
        }
    }

    for(auto&e : edgeBuilder.edges()){
        node_to_node[e[0]].insert(e[1]);
        node_to_node[e[1]].insert(e[0]);
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
