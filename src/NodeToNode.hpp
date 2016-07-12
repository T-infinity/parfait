#include "VectorTools.h"
#include "CGNS.h"
#include "MessagePasser.h"

template<typename MeshType>
std::vector <std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::   buildNodeToNodeConnectivity() {
    printf("Rank(%i) Startint\n",MessagePasser::Rank()); fflush(stdout);
    for(int cellId = 0; cellId < mesh.numberOfTets(); cellId++){
        const int* ptr = mesh.getTet(cellId);
        std::array<int,4> cell {ptr[0],ptr[1],ptr[2],ptr[3]};
        for(int edge = 0; edge < CGNS::Edges::Tet::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Tet::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }
    printf("Rank(%i) 2\n",MessagePasser::Rank()); fflush(stdout);
    for(int cellId = 0; cellId < mesh.numberOfPyramids(); cellId++){
        auto cell = mesh.getPyramid(cellId);
        for(int edge = 0; edge < CGNS::Edges::Pyramid::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Pyramid::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }
    printf("Rank(%i) 3\n",MessagePasser::Rank()); fflush(stdout);
    printf("Rank(%i) number of prisms: %i\n",MessagePasser::Rank(),mesh.numberOfPrisms()); fflush(stdout);
    for(int cellId = 0; cellId < mesh.numberOfPrisms(); cellId++){
        auto cell = mesh.getPrism(cellId);
        printf("Rank %d, cell: %d %d %d %d %d %d\n",
               MessagePasser::Rank(),
               cell[0], cell[1], cell[2],
               cell[3], cell[4], cell[5]);
        fflush(stdout);
        printf("Number of edges: %d\n", CGNS::Edges::Prism::numberOfEdges()); fflush(stdout);
        for(int edge = 0; edge < CGNS::Edges::Prism::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Prism::getEdge(cell, edge);
            printf("Rank %d, edge %d %d\n", e[0], e[1]); fflush(stdout);
            addEdge(e[0], e[1]);
        }
    }
    printf("4\n"); fflush(stdout);
    for(int cellId = 0; cellId < mesh.numberOfHexes(); cellId++){
        auto cell = mesh.getHex(cellId);
        for(int edge = 0; edge < CGNS::Edges::Hex::numberOfEdges(); edge++){
            auto e = CGNS::Edges::Hex::getEdge(cell, edge);
            addEdge(e[0], e[1]);
        }
    }
    printf("5\n"); fflush(stdout);
    std::vector<std::vector<int>> n2n;
    for(auto& row:node_to_node)
        n2n.push_back(std::vector<int>(row.begin(),row.end()));
    return n2n;
}

template <typename MeshType>
void Parfait::NodeToNodeBuilder<MeshType>::addEdge(int left, int right) {
    node_to_node[right].insert(left);
    node_to_node[left].insert(right);
}