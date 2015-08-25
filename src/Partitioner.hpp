#include <MessagePasser.h>
#include <VectorTools.h>
#include <ParmetisWrapper.h>
#include <NodeToNode.h>


template<class MeshType>
Parfait::Partitioner<MeshType>::Partitioner(MeshType& m)
:mesh(m)
{
    NodeToNodeBuilder<MeshType> builder(mesh);
	connectivity = builder.buildNodeToNodeConnectivity();
}


template<class MeshType>
void Parfait::Partitioner<MeshType>::buildProcNodeMap() {
	procNodeMap.resize(MessagePasser::NumberOfProcesses());
	if(MessagePasser::Rank() == 0)
		printf("Allgather to build procNodeMap:\n");
	MessagePasser::AllGather(this->mesh.numberOfNodes(), this->procNodeMap);
	this->procNodeMap.insert(this->procNodeMap.begin(),0);
	for(int i=1;i<procNodeMap.size();i++)
		this->procNodeMap[i] += this->procNodeMap[i-1];
}

template<class MeshType>
std::vector<int> Parfait::Partitioner<MeshType>::generatePartVector()
{
	vector<int> part(mesh.numberOfNodes(),0);
	vector<int> ia(connectivity.size()+1,0);
	for(int i=0;i<connectivity.size();i++)
		ia[i+1] = ia[i] + connectivity[i].size();
	vector<int> ja;
	ja.reserve(ia.back());
	for(auto row:connectivity)
		for(int nbr:row)
			ja.push_back(nbr);
	PartitionMesh(MessagePasser::Rank(),MessagePasser::NumberOfProcesses(),procNodeMap.data(),
			ia.data(),ja.data(),part.data());
	return part;
}
