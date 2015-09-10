#include <MessagePasser.h>
#include <VectorTools.h>
#include <ParmetisWrapper.h>
#include <NodeToNode.h>

template <class Mesh>
Parfait::Partitioner<Mesh>::Partitioner(const Mesh & m)
:mesh(m)
{
    NodeToNodeBuilder<decltype(m)> builder(mesh);
	connectivity = builder.buildNodeToNodeConnectivity();
}

template <class Mesh>
std::vector<long> Parfait::Partitioner<Mesh>::buildProcNodeMap() {
	vector<long> procNodeMap(MessagePasser::NumberOfProcesses());
	MessagePasser::AllGather((long)mesh.numberOfOwnedNodes(), procNodeMap);
	procNodeMap.insert(procNodeMap.begin(),0);
	for(int i=1;i<procNodeMap.size();i++)
		procNodeMap[i] += procNodeMap[i-1];
    return procNodeMap;
}

template <class Mesh>
std::vector<int> Parfait::Partitioner<Mesh>::generatePartVector()
{
	vector<int> part(mesh.numberOfOwnedNodes(),0);
	vector<long> ia(connectivity.size()+1,0);
	for(long i=0;i<connectivity.size();i++)
		ia[i+1] = ia[i] + connectivity[i].size();
	vector<long> ja;
	ja.reserve(ia.back());
	for(auto& row:connectivity)
		for(auto nbr:row)
			ja.push_back(nbr);
    auto procNodeMap = buildProcNodeMap();
	PartitionMesh(MessagePasser::Rank(),MessagePasser::NumberOfProcesses(),procNodeMap.data(),
			ia.data(),ja.data(),part.data());
	return part;
}
