#ifndef NODE_TO_NODE_H
#define NODE_TO_NODE_H

#include <vector>
#include <set>
#include <algorithm>

namespace Parfait {
    template<typename MeshType>
    class NodeToNodeBuilder {
    public:
        NodeToNodeBuilder(MeshType& m)
                : mesh(m),
                node_to_node(mesh.numberOfNodes(),std::set<int>())
        {};

        std::vector<std::vector<int>> buildNodeToNodeConnectivity();
    private:
        MeshType& mesh;
        std::vector<std::set<int>> node_to_node;

        void processFace(int cell_id,int face_id);
        std::vector<std::vector<int>> returnSetsAsVectors();
    };

    #include "NodeToNode.hpp"
}
#endif
