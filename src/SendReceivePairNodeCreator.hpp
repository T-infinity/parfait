#include "ParallelMesh.h"
#include "SendReceivePairNodeCreator.h"
#include "MessagePasser.h"
namespace Parfait {

  namespace SendReceivePairCreator {

    inline NodePairCreator::NodePairCreator(std::shared_ptr<ParallelMesh> mesh_in)
            : mesh(mesh_in) {
        buildGlobalToLocalMap();
    }
    inline void NodePairCreator::buildGlobalToLocalMap() {
        for(unsigned int local = 0; local < mesh->metaData->globalNodeIds.size(); local++){
            auto & global = mesh->metaData->globalNodeIds[local];
            global_to_local[global] = local;
        }
    }

    inline SendReceivePair NodePairCreator::create() {
        num_local_nodes = mesh->countNodesAtDegree(0);
        num_ghost_nodes = mesh->countNodesAtDegree(0);
        int total_nodes = num_ghost_nodes + num_local_nodes;
        for(int local = 0; local < total_nodes; local++)
            pair.send[local] = std::vector<int>{};
        pair.recv.resize(total_nodes, -1);

        buildPair();
        return pair;
    }
    void NodePairCreator::buildPair() {
        std::vector<long> needed_nodes = findNeededNodes();
        for(int proc = 0; proc < MessagePasser::NumberOfProcesses(); proc++) {
            std::vector<long> remote_nodes;
            if(proc == MessagePasser::Rank())
                remote_nodes = getOwnedNodeGlobalIds();
            MessagePasser::Broadcast(remote_nodes, proc);
            std::vector<long> do_need_remote_nodes;
            if(proc != MessagePasser::Rank()) {
                do_need_remote_nodes = doNeedRemoteNodes(needed_nodes, remote_nodes);
                markRecvFromProc(do_need_remote_nodes, proc);
                MessagePasser::Send(do_need_remote_nodes, proc);
            }
            else {
                for(int remote_proc = 0; remote_proc < MessagePasser::NumberOfProcesses(); remote_proc++) {
                    if(remote_proc == MessagePasser::Rank()) continue;
                    std::vector<long> send_to_proc;
                    MessagePasser::Recv(send_to_proc, remote_proc);
                    for (auto global : send_to_proc)
                        if (global >= 0)
                            pair.send[global].push_back(remote_proc);
                }
            }

        }
    }
    inline std::vector<long> NodePairCreator::getOwnedNodeGlobalIds() {
        std::vector<long> owned_global_nodes;
        for(unsigned int localId = 0; localId < mesh->metaData->globalNodeIds.size(); localId++){
            if(mesh->metaData->nodeOwnershipDegree[localId] == 0) {
                auto global_id = mesh->metaData->globalNodeIds[localId];
                owned_global_nodes.push_back(global_id);
            }
        }
        return owned_global_nodes;
    }
    inline std::vector<long> NodePairCreator::findNeededNodes() {
        std::vector<long> needed_nodes;
        for(unsigned int localId = 0; localId < mesh->metaData->globalNodeIds.size(); localId++) {
            if (mesh->metaData->nodeOwnershipDegree[localId] != 0) {
                auto global_id = mesh->metaData->globalNodeIds[localId];
                needed_nodes.push_back(global_id);
            }
        }
        std::sort(needed_nodes.begin(), needed_nodes.end());
        return needed_nodes;
    }
    inline std::vector<long> NodePairCreator::doNeedRemoteNodes(const std::vector<long> &needed_nodes,
                                                                std::vector<long> &remote_nodes) {
        std::vector<long> respond(remote_nodes.size());
        for(unsigned int index = 0; index < remote_nodes.size(); index++){
            auto& global_id = remote_nodes[index];
            if(std::binary_search(needed_nodes.begin(), needed_nodes.end(), global_id))
                respond[index] = global_id;
            else
                respond[index] = -1;
        }
        return respond;
    }

    inline void NodePairCreator::markRecvFromProc(std::vector<long> &nodes_to_recv, int proc) {
        for(auto &global : nodes_to_recv)
            pair.recv[global] = proc;
    }
  }
}
