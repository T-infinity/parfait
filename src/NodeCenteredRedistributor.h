#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include <vector>
#include <MessagePasser.h>
#include <ParallelImportedUgrid.h>
#include <Fun3dMesh.h>
#include "MeshBasicParallel.h"

namespace Parfait {
	class ParallelMeshReDistributor {
	template<class T> using vector = std::vector<T>;
	public:
		ParallelMeshReDistributor(std::shared_ptr<MeshBasicParallel> mesh, std::vector<int> &part);

		std::shared_ptr<MeshBasicParallel> redistribute();
	private:
		void shuffleNodeIds();
		void shuffleTriangles();
		void shuffleQuads();
		void shuffleTets();
		void shufflePyramids();
		void shufflePrisms();
		void shuffleHexs();

		void identifyGhostNodes();
		void buildGlobalNodeIds();

		void shuffleNodeMetaData();
		int nproc;
		std::shared_ptr<MeshBasicParallel> mesh;
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

		void convertToLocalIds(std::vector<long>& ids);
        std::vector<int> convertToLocalIds(std::map<long,int> global_to_local_map,const std::vector<long>& ids);
	};

}
#include "NodeCenteredRedistributor.hpp"

#endif
#endif
