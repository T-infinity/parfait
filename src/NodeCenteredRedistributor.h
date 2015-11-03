#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include <vector>
#include <MessagePasser.h>
#include <ParallelImportedUgrid.h>
#include "ParallelMesh.h"

namespace Parfait {
	class NodeBasedRedistributor {
	template<class T> using vector = std::vector<T>;
	public:
		NodeBasedRedistributor(std::shared_ptr<ParallelMesh> mesh, std::vector<int> &part);

		std::shared_ptr<ParallelMesh> redistribute();
	private:
		std::vector<long> redistributeNodeIds();
		void redistributeTriangles(std::vector<long>& my_ghost_ids);
		void redistributeQuads(std::vector<long>& my_ghost_ids);
		void redistributeTets(std::vector<long>& my_ghost_ids);
		void redistributePyramids(std::vector<long>& my_ghost_ids);
		void redistributePrisms(std::vector<long>& my_ghost_ids);
		void redistributeHexes(std::vector<long>& my_ghost_ids);

		std::vector<long> identifyGhostNodes(std::vector<long>& my_ghost_ids);
		void buildGlobalNodeIds(std::vector<long>& my_ghost_ids);

		void redistributeNodeMetaData(std::vector<long>& my_ghost_ids);
		int nproc;
		std::shared_ptr<ParallelMesh> mesh;
		std::vector<int> part;
		std::vector<int> nodeMap;

		std::vector<long> ghostNodeIds;
		std::vector<int> recvAssociatedComponentIds;
		std::vector<double> recvXYZ;
		std::vector<long> recvTriangles;
		std::vector<int> recvTriangleTags;
		std::vector<long> recvQuads;
		std::vector<int> recvQuadTags;
		std::vector<long> recvTets;
		std::vector<long> recvPyramids;
		std::vector<long> recvPrisms;
		std::vector<long> recvHexs;

		std::vector<long> globalNodeIds;
		int getLocalNodeId(long globalNodeId,std::vector<long>& my_ghost_ids);

        std::map<long,int> global_to_local_map;

        std::vector<int> convertToLocalIds(std::map<long,int> global_to_local_map,const std::vector<long>& ids);
	};


}
#include "NodeCenteredRedistributor.hpp"

#endif
#endif
