#include "VectorTools.h"
#include "CGNS.h"

template<typename MeshType>
std::vector <std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::buildNodeToNodeConnectivity() {

    for(int cellId = 0; cellId < mesh.numberOfTets(); cellId++){
        auto cell = mesh.getTet(cellId);
        for(int edge = 0; edge < CGNS::Edges::Tet::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Tet::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }
    for(int cellId = 0; cellId < mesh.numberOfPyramids(); cellId++){
        auto cell = mesh.getPyramid(cellId);
        for(int edge = 0; edge < CGNS::Edges::Pyramid::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Pyramid::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }    
    for(int cellId = 0; cellId < mesh.numberOfPrisms(); cellId++){
        auto cell = mesh.getPrism(cellId);
        for(int edge = 0; edge < CGNS::Edges::Prism::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Prism::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }
    for(int cellId = 0; cellId < mesh.numberOfHexes(); cellId++){
        auto cell = mesh.getHex(cellId);
        for(int edge = 0; edge < CGNS::Edges::Hex::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Hex::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }
    return node_to_node;
}

template <typename MeshType>
void Parfait::NodeToNodeBuilder<MeshType>::addEdge(int left, int right) {
    insertUnique(node_to_node[right], left);
    insertUnique(node_to_node[left], right);
}