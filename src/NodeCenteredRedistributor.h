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
		void shuffleNodes();
		void shuffleTriangles();
		void shuffleQuads();
		void shuffleTets();
		void shufflePyramids();
		void shufflePrisms();
		void shuffleHexs();
		void identifyGhostNodes();
		void shuffleGhostNodes();

		int createNewParallelUgrid(std::vector<Parfait::MapbcReader> &mapbcVector);
	private:
		int nproc;
		Parfait::ParallelImportedUgrid &ugrid;
		std::vector<int> part;
		std::vector<int> nodeMap;

		std::vector<long> recvIds;
		std::vector<int> ghostIds;
		std::vector<int> componentIds;
		std::vector<double> recvNodes;
		std::vector<int> recvTriangles;
		std::vector<int> recvTriangleTags;
		std::vector<int> recvQuads;
		std::vector<int> recvQuadTags;
		std::vector<int> recvTets;
		std::vector<int> recvPyramids;
		std::vector<int> recvPrisms;
		std::vector<int> recvHexs;
		std::vector<int> recvGhostIds;
		std::vector<double> recvGhosts;
	};
}
#include "NodeCenteredRedistributor.hpp"

#endif
#endif
