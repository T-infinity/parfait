#ifndef NODE_TO_NODE_H
#define NODE_TO_NODE_H

#include <vector>
//#include <set>
//#include <algorithm>

namespace Parfait {
    template<typename MeshType>
    class NodeToNodeBuilder {
    public:
        NodeToNodeBuilder(MeshType& m)
                : mesh(m),
                node_to_node(mesh.numberOfNodesOfDegreeOrUnder(0),std::vector<long>())
        {};

        std::vector<std::vector<long>> buildNodeToNodeConnectivity();
    private:
        MeshType& mesh;
        std::vector<std::vector<long>> node_to_node;

        void processFace(int cell_id,int face_id);
        std::vector<std::vector<long>> returnSetsAsVectors();
    };

}
#include "NodeToNode.hpp"
#endif
