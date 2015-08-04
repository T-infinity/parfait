#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>

template<class MeshType>
class ParMetisPrepper
{
	template<class T> using vector = std::vector<T>;
	public:
		ParMetisPrepper() = delete;
		ParMetisPrepper(MeshType& mesh_in);
		void buildNodeToNodeConnectivity();
		vector<int> getPartVector();
		vector<vector<int> > connectivity;
		vector<int> procNodeMap;
	private:
		MeshType& mesh;
        vector<int> node_ids;
        vector<vector<int>> node_to_node;

		void buildProcNodeMap();
		vector<int> gatherNodeToNodeFromOtherProcs(int proc);
		vector<int> gatherNodeIdsFromOtherProcs(int proc);
		vector<int> gatherValenceCountsFromOtherProcs(int proc);
		vector<int> prepareSendIdsForProc(int proc);
		vector<int> prepareSendValenceForProc(int proc);
		vector<int> packUpSendNodeToNodeForProc(int proc);
		void combineConnectivitiesOnProc(int proc,  const vector<int> &ids_from_other_procs,
                                                      const vector<int> &valenceFromOtherProcs,
                                                      const vector<int> &globalNodeToNode);
};

#include "ParmetisPrepper.hpp"

#endif
