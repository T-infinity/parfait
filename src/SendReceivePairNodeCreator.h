#pragma once

#include <vector>
#include <memory>
#include <map>
#include "ParallelMesh.h"
namespace Parfait {

  namespace SendReceivePairCreator {
    class NodePairCreator {
    private:
        std::shared_ptr<ParallelMesh> mesh;
        int num_local_nodes;
        int num_ghost_nodes;
        std::map<long, int> global_to_local;
        SendReceivePair pair;

    public:
        NodePairCreator(std::shared_ptr<ParallelMesh> mesh);

        SendReceivePair create();
        std::vector<long> getOwnedNodeGlobalIds();
        std::vector<long> findNeededNodes();
        std::vector<long> doNeedRemoteNodes(const std::vector<long> &needed_nodes,
                                            std::vector<long> &remote_nodes);
        void buildPair();
    };


  }
}

#include "SendReceivePairNodeCreator.hpp"