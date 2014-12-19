#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include "imported_ugrid.h"
#include "message_passer.h"
#include "fun3d_mesh.h"
#include "mapbc_reader.h"
#include <vector>

using std::vector;

class ParallelMeshReDistributor
{
	public: 
		ParallelMeshReDistributor(ImportedUgrid &ugrid,vector<int> &gridNodeMap,vector<int> &part);
		void shuffleNodes();
		void shuffleTriangles();
		void shuffleQuads();
		void shuffleTets();
		void shufflePyramids();
		void shufflePrisms();
		void shuffleHexs();
		void identifyGhostNodes();
		void shuffleGhostNodes();

		Fun3DMesh createFun3DMesh(vector<MapbcReader> &mapbcVector);
	private:
		int nproc;
		ImportedUgrid &ugrid;
		vector<int> part;
		vector<int> nodeMap;
		vector<int> gridNodeMap;
		
		vector<int> recvIds;
		vector<int> ghostIds;
		vector<double> recvNodes;
		vector<int> recvTriangles;
		vector<int> recvTriangleTags;
		vector<int> recvQuads;
		vector<int> recvQuadTags;
		vector<int> recvTets;
		vector<int> recvPyramids;
		vector<int> recvPrisms;
		vector<int> recvHexs;
		vector<int> recvGhostIds;
		vector<double> recvGhosts;

		int localId(int globalId);
		int calcImesh(int localId);
};

#endif
#endif
