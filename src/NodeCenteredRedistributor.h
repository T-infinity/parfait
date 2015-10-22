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
		void redistributeNodeIds();
		void redistributeTriangles();
		void redistributeQuads();
		void redistributeTets();
		void redistributePyramids();
		void redistributePrisms();
		void redistributeHexes();

		void identifyGhostNodes();
		void buildGlobalNodeIds();

		void redistributeNodeMetaData();
		int nproc;
		std::shared_ptr<ParallelMesh> mesh;
		std::vector<int> part;
		std::vector<int> nodeMap;

		std::vector<long> recvNodeIds;
		std::vector<long> recvGhostNodeIds;
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
		int getLocalNodeId(long globalNodeId);

        std::map<long,int> global_to_local_map;

        std::vector<int> convertToLocalIds(std::map<long,int> global_to_local_map,const std::vector<long>& ids);
	};


}
#include "NodeCenteredRedistributor.hpp"

#endif
#endif
