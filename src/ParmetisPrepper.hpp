#include <MessagePasser.h>
#include <VectorTools.h>
#include <ParmetisWrapper.h>
#include <NodeToNode.h>


template<class MeshType>
ParMetisPrepper<MeshType>::ParMetisPrepper(MeshType& m)
:mesh(m)
{
	node_ids = Parfait::buildUniqueNodeList(mesh);

	int negative_count = 0;
	for(int i=0;i<node_ids.size();i++) {
		if (node_ids[i] < 0) {
			negative_count++;
		}
	}
	printf("Rank %i: negative count %i of %i\n",MessagePasser::Rank(),negative_count,node_ids.size());
	MessagePasser::Barrier();
	return;

	node_to_node = Parfait::buildNodeToNode(mesh,node_ids);

	for(auto& nbr_list:node_to_node){
		for(auto nbr:nbr_list)
			if(nbr<0)
				throw std::logic_error("negative nbr");
	}


	printf("--Rank %i done building local n2n\n",MessagePasser::Rank());
	connectivity.resize(mesh.numberOfNodes());
}

template<class MeshType>
void ParMetisPrepper<MeshType>::buildNodeToNodeConnectivity() {
	buildProcNodeMap();

	for(int proc=0;proc<MessagePasser::NumberOfProcesses();proc++) {

		auto ids_from_other_procs = gatherNodeIdsFromOtherProcs(proc);
    	auto valenceFromOtherProcs = gatherValenceCountsFromOtherProcs(proc);
		auto globalNodeToNode = gatherNodeToNodeFromOtherProcs(proc);

		combineConnectivitiesOnProc(proc, ids_from_other_procs, valenceFromOtherProcs, globalNodeToNode);
	}
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::gatherNodeToNodeFromOtherProcs(int proc) {
	if(MessagePasser::Rank() == proc)
		printf("Rank %i, gathering node to node connectivity from other procs\n",MessagePasser::Rank());
	auto sendNodeToNode = packUpSendNodeToNodeForProc(proc);
	vector<int> globalNodeToNode;
	MessagePasser::Gatherv(sendNodeToNode,globalNodeToNode,proc);
	return globalNodeToNode;
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::gatherValenceCountsFromOtherProcs(int proc) {
	auto sendValence = prepareSendValenceForProc(proc);
	vector<int> valenceFromOtherProcs;
	MessagePasser::Gatherv(sendValence,valenceFromOtherProcs,proc);
	return valenceFromOtherProcs;
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::gatherNodeIdsFromOtherProcs(int proc) {
	auto send_ids = prepareSendIdsForProc(proc);
	vector<int> ids_from_other_procs;
	MessagePasser::Gatherv(send_ids, ids_from_other_procs,proc);
	return ids_from_other_procs;
}

template<class MeshType>
void ParMetisPrepper<MeshType>::combineConnectivitiesOnProc(int proc, const vector<int> &ids_from_other_procs,
                                                      const vector<int> &valenceFromOtherProcs,
                                                      const vector<int> &globalNodeToNode) {
	if(MessagePasser::Rank() == proc) {
			printf("Rank %i, combining n2n from other procs\n",MessagePasser::Rank());
			int index = 0;
			int position = 0;
			for(int id:ids_from_other_procs) {
				int valence = valenceFromOtherProcs[index];
				for(int k=0;k<valence;k++) {
					int nbr = globalNodeToNode[position++];
					insertUnique(connectivity[id- this->procNodeMap[proc]],nbr);
				}
				index++;
			}
	}
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::packUpSendNodeToNodeForProc(int proc) {
	int index = 0;
	vector<int> sendNodeToNode;
	for(auto id:node_ids) {
		if(id >= procNodeMap[proc] && id < this->procNodeMap[proc+1])
			for(auto nbr:node_to_node[index])
				sendNodeToNode.push_back(nbr);
		index++;
	}
	return sendNodeToNode;
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::prepareSendValenceForProc(int proc) {
	vector<int> sendValence;
	int index=0;
	if(MessagePasser::Rank() == 0)
			printf("building valence map...\n");
	for(auto id:node_ids)
		{
			if(id >= procNodeMap[proc] && id < this->procNodeMap[proc+1])
				sendValence.push_back(node_to_node[index].size());
			index++;
		}
	return sendValence;
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::prepareSendIdsForProc(int proc) {
	vector<int> sendIds;
	for(auto id:node_ids)
			if(id >= procNodeMap[proc] && id < this->procNodeMap[proc+1])
				sendIds.push_back(id);
	return sendIds;
}

template<class MeshType>
void ParMetisPrepper<MeshType>::buildProcNodeMap() {
	procNodeMap.resize(MessagePasser::NumberOfProcesses());
	if(MessagePasser::Rank() == 0)
		printf("Allgather to build procNodeMap:\n");
	MessagePasser::AllGather(this->mesh.numberOfNodes(), this->procNodeMap);
	this->procNodeMap.insert(this->procNodeMap.begin(),0);
	for(int i=1;i<procNodeMap.size();i++)
		this->procNodeMap[i] += this->procNodeMap[i-1];
}

template<class MeshType>
std::vector<int> ParMetisPrepper<MeshType>::getPartVector()
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
