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

	shuffleNodes();
	shuffleTriangles();
	shuffleQuads();
	shuffleTets();
	shufflePyramids();
	shufflePrisms();
	shuffleHexs();

	identifyGhostNodes();
	shuffleGhostNodes();
}

inline void Parfait::ParallelMeshReDistributor::shuffleNodes()
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
				sendNodes.push_back(ugrid.nodes[3*localNodeId+0]);
				sendNodes.push_back(ugrid.nodes[3*localNodeId+1]);
				sendNodes.push_back(ugrid.nodes[3*localNodeId+2]);
                componentIds.push_back(ugrid.getNodeComponentId(localNodeId));
			}
		}
		MessagePasser::Gatherv(sendIds,recvIds,proc);
		MessagePasser::Gatherv(sendNodes,recvNodes,proc);
        MessagePasser::Gatherv(componentIds,componentIds,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shuffleTriangles()
{
	for(int proc:range(nproc)) {
        vector<long> sendTriangleIds;
        vector<long> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
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
			neededNodeIds = recvIds;
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
			neededNodeIds = recvIds;
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
			neededNodeIds = recvIds;
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
			neededNodeIds = recvIds;
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
			neededNodeIds = recvIds;
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

inline int Parfait::ParallelMeshReDistributor::createNewParallelUgrid(vector<MapbcReader> &mapbcVector)
{
	return 0;
}

inline void Parfait::ParallelMeshReDistributor::identifyGhostNodes()
{
	for(int id:recvTets)
		if(!std::binary_search(recvIds.begin(),recvIds.end(),id))
			ghostIds.push_back(id);
	for(int id:recvPyramids)
		if(!std::binary_search(recvIds.begin(),recvIds.end(),id))
			ghostIds.push_back(id);
	for(int id:recvPrisms)
		if(!std::binary_search(recvIds.begin(),recvIds.end(),id))
			ghostIds.push_back(id);
	for(int id:recvHexs)
		if(!std::binary_search(recvIds.begin(),recvIds.end(),id))
			ghostIds.push_back(id);

	if(!std::is_sorted(recvIds.begin(),recvIds.end()))
		abort();
	// sort ghost ids and remove duplicates
	std::sort(ghostIds.begin(),ghostIds.end());
	ghostIds.erase(unique(ghostIds.begin(),ghostIds.end()),ghostIds.end());
}

inline void Parfait::ParallelMeshReDistributor::shuffleGhostNodes(){
	using namespace MessagePasser;
	for(int proc:range(nproc)){
		vector<int> neededGhosts;
		vector<int> sendGhostIds;
		if(Rank() == proc)
			neededGhosts = ghostIds;

		// proc tells everyone what ghost nodes it needs
		Broadcast(neededGhosts,proc);

		// make a list of node ids that you have for proc
		for(int i:range(neededGhosts))
			if(neededGhosts[i] >= nodeMap[Rank()] && neededGhosts[i] < nodeMap[Rank()+1])
				sendGhostIds.push_back(neededGhosts[i]);

		// pack those nodes into a vector
		vector<double> sendGhosts;
		sendGhosts.reserve(3*sendGhostIds.size());
		for(int id:sendGhostIds){
			int index = id - nodeMap[Rank()];
			sendGhosts.push_back(ugrid.nodes[3*index+0]);
			sendGhosts.push_back(ugrid.nodes[3*index+1]);
			sendGhosts.push_back(ugrid.nodes[3*index+2]);
		}
		vector<int> tmpMap;
		Gatherv(sendGhostIds,recvGhostIds,tmpMap,proc);
		Gatherv(sendGhosts,recvGhosts,tmpMap,proc);
	}
}

#endif
