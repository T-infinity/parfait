#include "node_to_node.h"
#include "cell_to_cell.h"
#include "message_passer.h"
#include "parmetis_wrapper.h"
#include "range_loop.h"

using std::vector;
using namespace MessagePasser;

template<class MeshType>
void ParMetisPrepper<MeshType>::buildNodeToNodeConnectivity()
{
	int nproc = NumberOfProcesses();
	vector<int> nodeIds = buildUniqueNodeList(mesh);
	vector<vector<int> > nodeToNode = buildNodeToNode(mesh,nodeIds); 
	connectivity.resize(mesh.numberOfNodes());
	int nnodes = mesh.numberOfNodes();
	// build a map of which nodes are on each proc
	procNodeMap.resize(nproc);
	AllGather(mesh.numberOfNodes(),procNodeMap);
	procNodeMap.insert(procNodeMap.begin(),0);
	for(int i=1;i<=nproc;i++)
		procNodeMap[i] += procNodeMap[i-1];
	// combine local nodeToNode maps to get a full map
	vector<int> sendIds;
	vector<int> sendValence;
	vector<int> sendNodeToNode;
	for(int proc=0;proc<nproc;proc++)
	{
		if(Rank() == 0)
			printf("Sending partial n2n to proc %i\n",proc);
		// count how many nodes you have for proc
		sendIds.clear();
		for(auto id:nodeIds)
			if(id >= procNodeMap[proc] && id < procNodeMap[proc+1])
				sendIds.push_back(id);
		// tell proc
		vector<int> idsFromOtherProcs;
		vector<int> procMap;
		if(Rank() == 0)
			printf("Gathering ids from all procs....\n");
		Gatherv(sendIds,idsFromOtherProcs,procMap,proc);	
		// build map
		sendValence.clear();
		int index=0;
		if(Rank() == 0)
			printf("building valence map...\n");
		for(auto id:nodeIds)
		{
			if(id >= procNodeMap[proc] && id < procNodeMap[proc+1])
				sendValence.push_back(nodeToNode[index].size());
			index++;
		}
		// get maps on proc
		vector<int> valenceFromOtherProcs;
		if(Rank() == 0)
			printf("Gathering valence maps\n");
		Gatherv(sendValence,valenceFromOtherProcs,procMap,proc);
		// pack up nodeToNode for proc
		index = 0;
		sendNodeToNode.clear();
		if(Rank() == 0)
			printf("Packing up n2n\n");
		for(auto id:nodeIds)
		{
			if(id >= procNodeMap[proc] && id < procNodeMap[proc+1])
				for(auto nbr:nodeToNode[index])
					sendNodeToNode.push_back(nbr);
			index++;
		}	
		// get on proc
		vector<int> mapOfGlobalNodeToNode;
		vector<int> globalNodeToNode;
		if(Rank() == 0)
			printf("Gathering n2n\n");
		Gatherv(sendNodeToNode,globalNodeToNode,mapOfGlobalNodeToNode,proc);

		if(Rank() == proc)
		{
			index = 0;
			int position = 0;
			for(int id:idsFromOtherProcs)
			{
				int valence = valenceFromOtherProcs[index];
				for(int k=0;k<valence;k++)
				{
					int nbr = globalNodeToNode[position++];
					insertUnique(connectivity[id-procNodeMap[proc]],nbr);
				}
				index++;
			}
		}
	}
}

template<class MeshType>
vector<int> ParMetisPrepper<MeshType>::getPartVector()
{
	vector<int> part(mesh.numberOfNodes(),0);
	// build ia map
	vector<int> ia(connectivity.size()+1,0);
	for(int i=0;i<connectivity.size();i++)
		ia[i+1] = ia[i] + connectivity[i].size();
	// build ja map
	vector<int> ja;
	ja.reserve(ia.back());
	for(auto row:connectivity)
		for(int nbr:row)
			ja.push_back(nbr);
	PartitionMesh(Rank(),NumberOfProcesses(),procNodeMap.data(),
			ia.data(),ja.data(),part.data());
	return part;
}
