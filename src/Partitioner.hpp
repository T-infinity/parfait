#include <MessagePasser.h>
#include <VectorTools.h>
#include <ParmetisWrapper.h>
#include <NodeToNode.h>


Parfait::Partitioner::Partitioner(ParallelImportedUgrid& m)
:mesh(m)
{
    NodeToNodeBuilder<decltype(m)> builder(mesh);
	connectivity = builder.buildNodeToNodeConnectivity();
    buildProcNodeMap();
}


void Parfait::Partitioner::buildProcNodeMap() {
	procNodeMap.resize(MessagePasser::NumberOfProcesses());
	if(MessagePasser::Rank() == 0)
		printf("Allgather to build procNodeMap:\n");
	MessagePasser::AllGather((long)this->mesh.numberOfNodesOfDegreeOrUnder(0), this->procNodeMap);
	this->procNodeMap.insert(this->procNodeMap.begin(),0);
	for(int i=1;i<procNodeMap.size();i++)
		this->procNodeMap[i] += this->procNodeMap[i-1];
}

std::vector<int> Parfait::Partitioner::generatePartVector()
{
	vector<int> part(mesh.numberOfNodesOfDegreeOrUnder(0),0);
	vector<long> ia(connectivity.size()+1,0);
	for(long i=0;i<connectivity.size();i++)
		ia[i+1] = ia[i] + connectivity[i].size();
	vector<long> ja;
	ja.reserve(ia.back());
	for(auto& row:connectivity)
		for(auto nbr:row)
			ja.push_back(nbr);
    printf("procNodeMap %ld %ld\n",procNodeMap[0],procNodeMap[1]);
    fflush(stdout);
	PartitionMesh(MessagePasser::Rank(),MessagePasser::NumberOfProcesses(),procNodeMap.data(),
			ia.data(),ja.data(),part.data());
	return part;
}
