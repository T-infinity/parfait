#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include <vector>
#include <MessagePasser.h>
#include <ImportedUgrid.h>
#include <Fun3dMesh.h>

namespace Parfait {
	class ParallelMeshReDistributor {
	template<class T> using vector = std::vector<T>;
	public:
		ParallelMeshReDistributor(Parfait::ParallelImportedUgrid &ugrid, std::vector<int> &part);
		void shuffleNodes();
		void shuffleTriangles();
		void shuffleQuads();
		void shuffleTets();
		void shufflePyramids();
		void shufflePrisms();
		void shuffleHexs();
		void identifyGhostNodes();
		void shuffleGhostNodes();

		int createFun3DMesh(std::vector<Parfait::MapbcReader> &mapbcVector);
	private:
		int nproc;
		Parfait::ImportedUgrid &ugrid;
		std::vector<int> part;
		std::vector<int> nodeMap;

		std::vector<int> recvIds;
		std::vector<int> ghostIds;
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
#include "ParallelMeshRedistributor.hpp"

#endif
#endif
