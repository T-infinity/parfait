#ifndef NODE_TO_NODE_H
#define NODE_TO_NODE_H

#include <vector>

namespace Parfait {
    template<typename MeshType>
    class NodeToNodeBuilder {
    public:
        NodeToNodeBuilder(MeshType& m)
                : mesh(m),
                node_to_node(mesh.numberOfNodes())
        {};

        std::vector<std::vector<int>> buildNodeToNodeConnectivity();
    private:
        MeshType& mesh;
        void addEdge(int left, int right);
        std::vector<std::vector<int>> node_to_node;

        void processFace(int cell_id,int face_id);
    };


}
#include "NodeToNode.hpp"
#endif
