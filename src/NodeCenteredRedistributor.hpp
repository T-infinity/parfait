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
	for(int proc:range(nproc))
	{
		int count = 0;
		for(int owner:part)
			if(owner == proc)
				count++;
		vector<int> sendIds;
		vector<double> sendNodes;
		sendIds.reserve(count);
		sendNodes.reserve(3*count);
		for(int i=0;i<part.size();i++)
		{
			if(part[i] == proc)
			{
				sendIds.push_back(i+nodeMap[MessagePasser::Rank()]);
				sendNodes.push_back(ugrid.nodes[3*i+0]);
				sendNodes.push_back(ugrid.nodes[3*i+1]);
				sendNodes.push_back(ugrid.nodes[3*i+2]);
			}
		}
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendIds,recvIds,tmpMap,proc);
		MessagePasser::Gatherv(sendNodes,recvNodes,tmpMap,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shuffleTriangles()
{
	vector<int> sendTriangleIds;
	sendTriangleIds.reserve(3*ugrid.triangles.size());
	for(int proc:range(nproc))
	{
		sendTriangleIds.clear();
		vector<int> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
		MessagePasser::Broadcast(neededNodeIds,proc);
		// loop over triangles
		for(int i=0;i<ugrid.triangles.size()/3;i++)
		{
			// check if proc owns any nodes in the triangle
			for(int j:range(3))
			{
				int globalId = ugrid.triangles[3*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendTriangleIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendTriangles;
		sendTriangles.reserve(3*sendTriangleIds.size());
		vector<int> sendTriangleTags;
		sendTriangleTags.reserve(sendTriangleIds.size());
		for(int id:sendTriangleIds)
		{
			sendTriangleTags.push_back(ugrid.triangleTags[id]);
			for(int j:range(3))
				sendTriangles.push_back(ugrid.triangles[3*id+j]);
		}
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendTriangles,recvTriangles,tmpMap,proc);
		MessagePasser::Gatherv(sendTriangleTags,recvTriangleTags,tmpMap,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shuffleQuads()
{
	vector<int> sendQuadIds;
	sendQuadIds.reserve(4*ugrid.quads.size());
	for(int proc:range(nproc))
	{
		sendQuadIds.clear();
		vector<int> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
		MessagePasser::Broadcast(neededNodeIds,proc);
		// loop over quads
		for(int i=0;i<ugrid.quads.size()/4;i++)
		{
			// check if proc owns any nodes in the quad
			for(int j:range(4))
			{
				int globalId = ugrid.quads[4*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendQuadIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendQuads;
		sendQuads.reserve(4*sendQuadIds.size());
		vector<int> sendQuadTags;
		sendQuadTags.reserve(sendQuadIds.size());
		for(int id:sendQuadIds)
		{
			sendQuadTags.push_back(ugrid.quadTags[id]);
			for(int j:range(4))
				sendQuads.push_back(ugrid.quads[4*id+j]);
		}
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendQuads,recvQuads,tmpMap,proc);
		MessagePasser::Gatherv(sendQuadTags,recvQuadTags,tmpMap,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shuffleTets()
{
	vector<int> sendTetIds;
	sendTetIds.reserve(4*ugrid.tets.size());
	for(int proc=0;proc<nproc;proc++) {
		sendTetIds.clear();
		vector<int> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
		MessagePasser::Broadcast(neededNodeIds,proc);
		for(int i=0;i<ugrid.tets.size()/4;i++) {
			for(int j=0;j<4;j++) {
				//long globalId = ugrid.get
				int globalId = ugrid.tets[4*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId)) {
					sendTetIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendTets;
		sendTets.reserve(4*sendTetIds.size());
		for(int id:sendTetIds)
			for(int j:range(4))
				sendTets.push_back(ugrid.tets[4*id+j]);
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendTets,recvTets,tmpMap,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shufflePyramids()
{
	vector<int> sendPyramidIds;
	sendPyramidIds.reserve(5*ugrid.pyramids.size());
	for(int proc:range(nproc))
	{
		sendPyramidIds.clear();
		vector<int> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
		MessagePasser::Broadcast(neededNodeIds,proc);
		// loop over pyramids
		for(int i=0;i<ugrid.pyramids.size()/5;i++)
		{
			// check if proc owns any nodes in the pyramid
			for(int j:range(5))
			{
				int globalId = ugrid.pyramids[5*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendPyramidIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendPyramids;
		sendPyramids.reserve(5*sendPyramidIds.size());
		for(int id:sendPyramidIds)
			for(int j:range(5))
				sendPyramids.push_back(ugrid.pyramids[5*id+j]);
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendPyramids,recvPyramids,tmpMap,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shufflePrisms()
{
	vector<int> sendPrismIds;
	sendPrismIds.reserve(6*ugrid.prisms.size());
	for(int proc:range(nproc))
	{
		sendPrismIds.clear();
		vector<int> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
		MessagePasser::Broadcast(neededNodeIds,proc);
		// loop over prisms
		for(int i=0;i<ugrid.prisms.size()/6;i++)
		{
			// check if proc owns any nodes in the prism
			for(int j:range(6))
			{
				int globalId = ugrid.prisms[6*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendPrismIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendPrisms;
		sendPrisms.reserve(6*sendPrismIds.size());
		for(int id:sendPrismIds)
			for(int j:range(6))
				sendPrisms.push_back(ugrid.prisms[6*id+j]);
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendPrisms,recvPrisms,tmpMap,proc);
	}
}

inline void Parfait::ParallelMeshReDistributor::shuffleHexs()
{
	vector<int> sendHexIds;
	sendHexIds.reserve(8*ugrid.hexs.size());
	for(int proc:range(nproc))
	{
		sendHexIds.clear();
		vector<int> neededNodeIds;
		if(MessagePasser::Rank() == proc)
			neededNodeIds = recvIds;
		MessagePasser::Broadcast(neededNodeIds,proc);
		// loop over hexs
		for(int i=0;i<ugrid.hexs.size()/8;i++)
		{
			// check if proc owns any nodes in the hex
			for(int j:range(8))
			{
				int globalId = ugrid.hexs[8*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendHexIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendHexs;
		sendHexs.reserve(8*sendHexIds.size());
		for(int id:sendHexIds)
			for(int j:range(8))
				sendHexs.push_back(ugrid.hexs[8*id+j]);
		vector<int> tmpMap;
		MessagePasser::Gatherv(sendHexs,recvHexs,tmpMap,proc);
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
