#ifndef GLOBAL_ID_TRANSLATOR_H
#define GLOBAL_ID_TRANSLATOR_H
#include <vector>
#include "message_passer.h"

using std::vector;

class GlobalIdTranslator
{
	public:
		template<typename MeshType>
		GlobalIdTranslator(MeshType &mesh)
		{
			vector<int> tmp(NumberOfProcesses(),0);				
			tmp[Rank()] = mesh.numberOfNodes();
			auto nodesPerProc = ParallelMax(tmp,0);
			Broadcast(nodesPerProc,0);
			nodeOffset = 0;
			for(int i=0;i<Rank();i++)
				nodeOffset += nodesPerProc[i];
		}
		int globalNodeId(int localNodeId)
		{
			return localNodeId + nodeOffset;
		}

		int localNodeId(int globalNodeId)
		{
			return globalNodeId - nodeOffset;
		}
	private:
		int nodeOffset;
};

#endif
