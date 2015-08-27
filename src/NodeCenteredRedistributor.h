#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include <vector>
#include <MessagePasser.h>
#include <ParallelImportedUgrid.h>
#include <Fun3dMesh.h>

namespace Parfait {
	class ParallelMeshReDistributor {
	template<class T> using vector = std::vector<T>;
	public:
		ParallelMeshReDistributor(Parfait::ParallelImportedUgrid &ugrid, std::vector<int> &part);
		//TODO: Maybe just scrap this and rewrite, otherwise, salvage...
		void shuffleNodeIds();
		void shuffleTriangles();
		void shuffleQuads();
		void shuffleTets();
		void shufflePyramids();
		void shufflePrisms();
		void shuffleHexs();

		void identifyGhostNodes();
		void buildGlobalNodeIds();

		void shuffleXYZ();


		Parfait::ParallelImportedUgrid createNewParallelUgrid();
	private:
		int nproc;
		Parfait::ParallelImportedUgrid &ugrid;
		std::vector<int> part;
		std::vector<int> nodeMap;

		std::vector<long> recvNodeIds;
		std::vector<long> recvGhostNodeIds;
		std::vector<int> componentIds;
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
	};

}
#include "NodeCenteredRedistributor.hpp"

#endif
#endif
