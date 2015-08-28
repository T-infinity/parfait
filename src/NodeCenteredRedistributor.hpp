#include <algorithm>
#include <RangeLoop.h>

#ifdef PARFAIT_WITH_MPI

inline Parfait::ParallelMeshReDistributor::ParallelMeshReDistributor(ParallelImportedUgrid &ugrid_in,
                                                                     vector<int> &part_in)
        : ugrid(ugrid_in),
          part(part_in)
{
    nproc = MessagePasser::NumberOfProcesses();
    nodeMap.assign(nproc,0);
    MessagePasser::AllGather(ugrid.numberOfNodes(),nodeMap);
    nodeMap.insert(nodeMap.begin(),0);
    for(int i=2;i<nodeMap.size();i++)
        nodeMap[i] += nodeMap[i-1];

    shuffleNodeIds();
    shuffleTriangles();
    shuffleQuads();
    shuffleTets();
    shufflePyramids();
    shufflePrisms();
    shuffleHexs();

    identifyGhostNodes();
    buildGlobalNodeIds();
    shuffleNodeMetaData();


}

std::vector<int> ParallelMeshReDistributor::convertToLocalIds(std::map<long,int> global_to_local_map,const std::vector<long>& ids){
    std::vector<int> local_ids;
    for(auto id:ids)
        local_ids.push_back(global_to_local_map[id]);
    return local_ids;
}

int ParallelMeshReDistributor::getLocalNodeId(long globalNodeId) {
    auto it = std::lower_bound(recvNodeIds.begin(), recvNodeIds.end(), globalNodeId);
    if(it == recvNodeIds.end()){
        it = std::lower_bound(recvGhostNodeIds.begin(),recvGhostNodeIds.end(),globalNodeId);
            if(it != recvGhostNodeIds.end())
                return std::distance(recvGhostNodeIds.begin(),it)+recvNodeIds.size();
            else
                throw std::logic_error("saldfjsdf");
    }
    return std::distance(recvNodeIds.begin(),it);
}

inline void ParallelMeshReDistributor::shuffleNodeMetaData() {
    std::map<long,int> global_to_local;
    for(int i=0;i<ugrid.globalNodeIds.size();i++)
        global_to_local.insert(std::make_pair(ugrid.globalNodeIds[i],i));
    for(int proc:range(nproc)) {
        vector<long> neededXYZ;
        if(MessagePasser::Rank() == proc)
            neededXYZ = globalNodeIds;
        MessagePasser::Broadcast(neededXYZ, proc);
        vector<double> sendXYZ;
        vector<long> sendGlobalNodeId;
        vector<int> sendAssociatedComponentId;
        for(auto globalNodeId : neededXYZ){
            auto it = global_to_local.find(globalNodeId);
            if(not (it == global_to_local.end())){
                auto localNodeId = it->second;
                sendXYZ.push_back(ugrid.nodes[3*localNodeId+0]);
                sendXYZ.push_back(ugrid.nodes[3*localNodeId+1]);
                sendXYZ.push_back(ugrid.nodes[3*localNodeId+2]);
                sendGlobalNodeId.push_back(globalNodeId);
                sendAssociatedComponentId.push_back(ugrid.getNodeComponentId(localNodeId));
            }
        }

        std::vector<double> just_recv_xyz;
        MessagePasser::Gatherv(sendXYZ, just_recv_xyz,proc);
        std::vector<long> just_recv_xyz_global_node_ids;
        MessagePasser::Gatherv(sendGlobalNodeId, just_recv_xyz_global_node_ids, proc);
        std::vector<int> just_recv_associated_component_ids;
        MessagePasser::Gatherv(sendAssociatedComponentId,just_recv_associated_component_ids,proc);
        if(MessagePasser::Rank() == proc){
            recvXYZ.resize(3*just_recv_xyz_global_node_ids.size());
            recvAssociatedComponentIds.resize(just_recv_associated_component_ids.size());
            for(int index = 0; index < just_recv_xyz_global_node_ids.size(); index++){
                auto globalNodeId = just_recv_xyz_global_node_ids[index];
                int localId = getLocalNodeId(globalNodeId);
                for(int i = 0; i < 3; i++)
                    recvXYZ[3*localId+i] = just_recv_xyz[3*index+i];
                recvAssociatedComponentIds[localId] = just_recv_associated_component_ids[index];
            }
        }
    }
}

inline void Parfait::ParallelMeshReDistributor::shuffleNodeIds()
{
    for(int proc:range(nproc)) {
        int count = 0;
        for(int owner:part)
            if(owner == proc)
                count++;
        vector<long> sendIds;
        vector<int> componentIds;
        vector<double> sendNodes;
        sendIds.reserve(count);
        componentIds.reserve(count);
        sendNodes.reserve(3*count);
        for(int localNodeId=0;localNodeId<part.size();localNodeId++) {
            if(part[localNodeId] == proc) {
                auto globalNodeId = ugrid.getGlobalNodeId(localNodeId);
                sendIds.push_back(globalNodeId);
            }
        }
        MessagePasser::Gatherv(sendIds, recvNodeIds,proc);
    }
    std::sort(recvNodeIds.begin(), recvNodeIds.end());
}

inline void Parfait::ParallelMeshReDistributor::shuffleTriangles()
{
    for(int proc:range(nproc)) {
        vector<long> sendTriangleIds;
        vector<long> neededNodeIds;
        if(MessagePasser::Rank() == proc)
            neededNodeIds = recvNodeIds;
        MessagePasser::Broadcast(neededNodeIds,proc);
        // loop over triangles
        for(int localCellId=0;localCellId<ugrid.triangles.size()/3;localCellId++) {
            for(int j:range(3)) {
                int localNodeId = ugrid.triangles[3*localCellId+j];
                auto globalId = ugrid.getGlobalNodeId(localNodeId);
                if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId)) {
                    sendTriangleIds.push_back(localCellId);
                    break;
                }
            }
        }
        vector<long> sendTriangles;
        vector<int> sendTriangleTags;
        for(auto & localCellId:sendTriangleIds) {
            sendTriangleTags.push_back(ugrid.triangleTags[localCellId]);
            for(int j:range(3)) {
                int localNodeId = ugrid.triangles[3*localCellId+j];
                auto globalId = ugrid.getGlobalNodeId(localNodeId);
                sendTriangles.push_back(globalId);
            }
        }
        vector<int> tmpMap;
        MessagePasser::Gatherv(sendTriangles,recvTriangles,tmpMap,proc);
        MessagePasser::Gatherv(sendTriangleTags,recvTriangleTags,tmpMap,proc);
    }
}

inline void Parfait::ParallelMeshReDistributor::shuffleQuads()
{
    for(int proc:range(nproc)) {
        vector<long> sendQuadIds;
        vector<long> neededNodeIds;
        if(MessagePasser::Rank() == proc)
            neededNodeIds = recvNodeIds;
        MessagePasser::Broadcast(neededNodeIds,proc);
        for(int localCellId=0;localCellId<ugrid.quads.size()/4;localCellId++) {
            for(int j:range(4)) {
                int localNodeId = ugrid.quads[4*localCellId+j];
                auto globalNodeId = ugrid.getGlobalNodeId(localNodeId);
                if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalNodeId)) {
                    sendQuadIds.push_back(localCellId);
                    break;
                }
            }
        }
        vector<long> sendQuads;
        vector<int> sendQuadTags;
        for(const auto & localCellId :sendQuadIds) {
            sendQuadTags.push_back(ugrid.quadTags[localCellId]);
            for(int j:range(4)) {
                int localNodeId = ugrid.quads[4 * localCellId + j];
                auto globalNodeId = ugrid.getGlobalNodeId(localNodeId);
                sendQuads.push_back(globalNodeId);
            }
        }
        MessagePasser::Gatherv(sendQuads,recvQuads,proc);
        MessagePasser::Gatherv(sendQuadTags,recvQuadTags,proc);
    }
}

inline void Parfait::ParallelMeshReDistributor::shuffleTets()
{
    vector<long> sendTetIds;
    sendTetIds.reserve(4*ugrid.tets.size());
    for(int proc=0;proc<nproc;proc++) {
        sendTetIds.clear();
        vector<long> neededNodeIds;
        if(MessagePasser::Rank() == proc)
            neededNodeIds = recvNodeIds;
        MessagePasser::Broadcast(neededNodeIds,proc);
        for(int i=0;i<ugrid.tets.size()/4;i++) {
            for(int j=0;j<4;j++) {
                long globalId = ugrid.getGlobalNodeId(ugrid.tets[4*i+j]);
                if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId)) {
                    sendTetIds.push_back(i);
                    break;
                }
            }
        }
        vector<long> sendTets;
        sendTets.reserve(4*sendTetIds.size());
        for(auto id:sendTetIds)
            for(int j:range(4))
                sendTets.push_back(ugrid.getGlobalNodeId(ugrid.tets[4*id+j]));
        MessagePasser::Gatherv(sendTets,recvTets,proc);
    }
}

inline void Parfait::ParallelMeshReDistributor::shufflePyramids()
{
    vector<long> sendPyramidIds;
    sendPyramidIds.reserve(5*ugrid.pyramids.size());
    for(int proc:range(nproc))
    {
        sendPyramidIds.clear();
        vector<long> neededNodeIds;
        if(MessagePasser::Rank() == proc)
            neededNodeIds = recvNodeIds;
        MessagePasser::Broadcast(neededNodeIds,proc);
        // loop over pyramids
        for(int i=0;i<ugrid.pyramids.size()/5;i++)
        {
            // check if proc owns any nodes in the pyramid
            for(int j:range(5))
            {
                long globalId = ugrid.getGlobalNodeId(ugrid.pyramids[5*i+j]);
                if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
                {
                    sendPyramidIds.push_back(i);
                    break;
                }
            }
        }
        vector<long> sendPyramids;
        sendPyramids.reserve(5*sendPyramidIds.size());
        for(auto id:sendPyramidIds)
            for(int j:range(5))
                sendPyramids.push_back(ugrid.getGlobalNodeId(ugrid.pyramids[5*id+j]));
        MessagePasser::Gatherv(sendPyramids,recvPyramids,proc);
    }
}

inline void Parfait::ParallelMeshReDistributor::shufflePrisms()
{
    vector<long> sendPrismIds;
    sendPrismIds.reserve(6*ugrid.prisms.size());
    for(int proc:range(nproc))
    {
        sendPrismIds.clear();
        vector<long> neededNodeIds;
        if(MessagePasser::Rank() == proc)
            neededNodeIds = recvNodeIds;
        MessagePasser::Broadcast(neededNodeIds,proc);
        // loop over prisms
        for(int i=0;i<ugrid.prisms.size()/6;i++)
        {
            // check if proc owns any nodes in the prism
            for(int j:range(6))
            {
                long globalId = ugrid.getGlobalNodeId(ugrid.prisms[6*i+j]);
                if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
                {
                    sendPrismIds.push_back(i);
                    break;
                }
            }
        }
        vector<long> sendPrisms;
        sendPrisms.reserve(6*sendPrismIds.size());
        for(auto id:sendPrismIds)
            for(int j:range(6))
                sendPrisms.push_back(ugrid.getGlobalNodeId(ugrid.prisms[6*id+j]));
        MessagePasser::Gatherv(sendPrisms,recvPrisms,proc);
    }
}

inline void Parfait::ParallelMeshReDistributor::shuffleHexs()
{
    vector<long> sendHexIds;
    sendHexIds.reserve(8*ugrid.hexs.size());
    for(int proc:range(nproc))
    {
        sendHexIds.clear();
        vector<long> neededNodeIds;
        if(MessagePasser::Rank() == proc)
            neededNodeIds = recvNodeIds;
        MessagePasser::Broadcast(neededNodeIds,proc);
        // loop over hexs
        for(int i=0;i<ugrid.hexs.size()/8;i++)
        {
            // check if proc owns any nodes in the hex
            for(int j:range(8))
            {
                long globalId = ugrid.getGlobalNodeId(ugrid.hexs[8*i+j]);
                if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
                {
                    sendHexIds.push_back(i);
                    break;
                }
            }
        }
        vector<long> sendHexs;
        sendHexs.reserve(8*sendHexIds.size());
        for(auto id:sendHexIds)
            for(int j:range(8))
                sendHexs.push_back(ugrid.getGlobalNodeId(ugrid.hexs[8*id+j]));
        MessagePasser::Gatherv(sendHexs,recvHexs,proc);
    }
}

inline Parfait::ParallelImportedUgrid Parfait::ParallelMeshReDistributor::createNewParallelUgrid() {
    std::vector<int> ownership_degree(globalNodeIds.size(),0);
    std::fill(ownership_degree.begin()+recvNodeIds.size(),ownership_degree.end(),1);

    std::map<long,int> global_to_local;
    for(int i=0;i<ugrid.globalNodeIds.size();i++)
        global_to_local.insert(std::make_pair(ugrid.globalNodeIds[i],i));

    return ParallelImportedUgrid(ugrid.globalNodeCount,globalNodeIds,ownership_degree,
                                 recvAssociatedComponentIds, recvXYZ,
                                 convertToLocalIds(global_to_local,recvTriangles),
                                 convertToLocalIds(global_to_local,recvQuads),
                                 convertToLocalIds(global_to_local,recvTets),
                                 convertToLocalIds(global_to_local,recvPyramids),
                                 convertToLocalIds(global_to_local,recvPrisms),
                                 convertToLocalIds(global_to_local,recvHexs),
                                 recvTriangleTags,recvQuadTags);
}

inline void Parfait::ParallelMeshReDistributor::identifyGhostNodes() {
    std::set<long> uniqueGhostNodeIds;
    if(!std::is_sorted(recvNodeIds.begin(), recvNodeIds.end()))
        throw std::logic_error("Recv node Ids expected in order.");
    for(auto id:recvTets)
        if(!std::binary_search(recvNodeIds.begin(), recvNodeIds.end(),id))
            uniqueGhostNodeIds.insert(id);
    for(auto id:recvPyramids)
        if(!std::binary_search(recvNodeIds.begin(), recvNodeIds.end(),id))
            uniqueGhostNodeIds.insert(id);
    for(auto id:recvPrisms)
        if(!std::binary_search(recvNodeIds.begin(), recvNodeIds.end(),id))
            uniqueGhostNodeIds.insert(id);
    for(auto id:recvHexs)
        if(!std::binary_search(recvNodeIds.begin(), recvNodeIds.end(),id))
            uniqueGhostNodeIds.insert(id);

    recvGhostNodeIds = std::vector<long>(uniqueGhostNodeIds.begin(), uniqueGhostNodeIds.end());
}

void ParallelMeshReDistributor::buildGlobalNodeIds() {
    globalNodeIds = recvNodeIds;
    globalNodeIds.insert(globalNodeIds.end(), recvGhostNodeIds.begin(), recvGhostNodeIds.end());
}

#endif
