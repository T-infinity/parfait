#include "parallel_mesh_redistributor.h"
#include "range_loop.h"
#include <algorithm>
#ifdef PARFAIT_WITH_MPI
using namespace MessagePasser;

void ParallelMeshReDistributor::identifyGhostNodes()
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

void ParallelMeshReDistributor::shuffleGhostNodes()
{

	for(int proc:range(nproc))
	{
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
		for(int id:sendGhostIds)
		{
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
