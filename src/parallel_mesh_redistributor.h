#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include "imported_ugrid.h"
#include "message_passer.h"
#include "fun3d_mesh.h"
#include "mapbc_reader.h"
#include <vector>

namespace Parfait {
	class ParallelMeshReDistributor {
	public:
		ParallelMeshReDistributor(Parfait::ImportedUgrid &ugrid, std::vector<int> &gridNodeMap, std::vector<int> &part);
		void shuffleNodes();
		void shuffleTriangles();
		void shuffleQuads();
		void shuffleTets();
		void shufflePyramids();
		void shufflePrisms();
		void shuffleHexs();
		void identifyGhostNodes();
		void shuffleGhostNodes();

		Parfait::Fun3DMesh createFun3DMesh(std::vector<Parfait::MapbcReader> &mapbcVector);
	private:
		int nproc;
		Parfait::ImportedUgrid &ugrid;
		std::vector<int> part;
		std::vector<int> nodeMap;
		std::vector<int> gridNodeMap;

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

		int localId(int globalId);
		int calcImesh(int localId);
	};
}
#include "parallel_mesh_redistributor.hpp"

#endif
#endif
