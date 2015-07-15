#ifndef GENERIC_NODE_H
#define GENERIC_NODE_H

namespace Parfait {
    template<class MeshType>
    class Node {
    public:
        Node(MeshType &mesh, int nodeId);

        int Id() { return nodeId; }

    private:
        MeshType &mesh;
        int nodeId;
    };
}
#include "GenericNode.hpp"

#endif
