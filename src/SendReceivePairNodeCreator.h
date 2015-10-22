#pragma once

#include <vector>
#include <memory>
#include "ParallelMesh.h"
namespace Parfait {

  namespace SendReceivePairCreator {
    class NodePairCreator {
    private:
        std::shared_ptr<ParallelMesh> mesh;
        int num_local_nodes;
        int num_ghost_nodes;
    public:
        NodePairCreator(std::shared_ptr<ParallelMesh> mesh);

        SendReceivePair create();
        std::vector<long> getOwnedNodeGlobalIds();
        std::vector<long> findNeededNodes();
    };
  }
}

#include "SendReceivePairNodeCreator.hpp"