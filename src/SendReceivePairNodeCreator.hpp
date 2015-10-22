#include "ParallelMesh.h"
#include "SendReceivePairNodeCreator.h"
#include "MessagePasser.h"
namespace Parfait {

  namespace SendReceivePairCreator {

    inline NodePairCreator::NodePairCreator(std::shared_ptr<ParallelMesh> mesh_in)
            : mesh(mesh_in) { }

    inline SendReceivePair NodePairCreator::create() {
        SendReceivePair pair;
        num_local_nodes = mesh->countNodesAtDegree(0);
        num_ghost_nodes = mesh->countNodesAtDegree(0);
        pair.send.resize(num_local_nodes);
        pair.recv.resize(num_ghost_nodes);

        std::vector<long> needed_nodes = findNeededNodes();
        for(int proc = 0; proc < MessagePasser::NumberOfProcesses(); proc++) {
            std::vector<long> owned_global_nodes;
            if(proc == MessagePasser::Rank())
                owned_global_nodes = getOwnedNodeGlobalIds();
            MessagePasser::Broadcast(owned_global_nodes, proc);
        }
        return pair;
    }

    inline std::vector<long> NodePairCreator::getOwnedNodeGlobalIds() {
        std::vector<long> owned_global_nodes;
        for(int localId = 0; localId < mesh->metaData->globalNodeIds.size(); localId++){
            if(mesh->metaData->nodeOwnershipDegree[localId] == 0) {
                auto global_id = mesh->metaData->globalNodeIds[localId];
                owned_global_nodes.push_back(global_id);
            }
        }
        return owned_global_nodes;
    }
    std::vector<long> NodePairCreator::findNeededNodes() {
        std::vector<long> needed_nodes;
        for(int localId = 0; localId < mesh->metaData->globalNodeIds.size(); localId++) {
            if (mesh->metaData->nodeOwnershipDegree[localId] != 0) {
                auto global_id = mesh->metaData->globalNodeIds[localId];
                needed_nodes.push_back(global_id);
            }
        }
        std::sort(needed_nodes.begin(), needed_nodes.end());
        return needed_nodes;
    }
  }
}
