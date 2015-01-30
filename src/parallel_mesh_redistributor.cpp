#include "parallel_mesh_redistributor.h"
#include "range_loop.h"
#include <algorithm>

#ifdef PARFAIT_WITH_MPI
using namespace MessagePasser;

ParallelMeshReDistributor::ParallelMeshReDistributor(ImportedUgrid &ugrid_in,vector<int> &gridNodeMap_in,
		vector<int> &part_in)
	: ugrid(ugrid_in),
	part(part_in),
	gridNodeMap(gridNodeMap_in)
{
	nproc = NumberOfProcesses();
	nodeMap.assign(nproc,0);
	AllGather(ugrid.numberOfNodes(),nodeMap);
	nodeMap.insert(nodeMap.begin(),0);
	for(int i=2;i<nodeMap.size();i++)
		nodeMap[i] += nodeMap[i-1];

	shuffleNodes();
	shuffleTriangles();
	shuffleQuads();
	shuffleTets();
	shufflePyramids();
	shufflePrisms();
	shuffleHexs();

	identifyGhostNodes();
	shuffleGhostNodes();
}

void ParallelMeshReDistributor::shuffleNodes()
{
	for(int proc:range(nproc))
	{
		int count = 0;
		for(int owner:part)
			if(owner == proc)
				count++;
		vector<int> sendIds;
		vector<double> sendNodes;
		sendIds.reserve(count);
		sendNodes.reserve(3*count);
		for(int i=0;i<part.size();i++)
		{
			if(part[i] == proc)
			{
				sendIds.push_back(i+nodeMap[Rank()]);
				sendNodes.push_back(ugrid.nodes[3*i+0]);
				sendNodes.push_back(ugrid.nodes[3*i+1]);
				sendNodes.push_back(ugrid.nodes[3*i+2]);
			}	
		}
		vector<int> tmpMap;
		Gatherv(sendIds,recvIds,tmpMap,proc);
		Gatherv(sendNodes,recvNodes,tmpMap,proc);
	}	
}

void ParallelMeshReDistributor::shuffleTriangles()
{
	vector<int> sendTriangleIds;
	sendTriangleIds.reserve(3*ugrid.triangles.size());
	for(int proc:range(nproc))
	{
		sendTriangleIds.clear();
		vector<int> neededNodeIds;
		if(Rank() == proc)
			neededNodeIds = recvIds;
		Broadcast(neededNodeIds,proc);
		// loop over triangles
		for(int i=0;i<ugrid.triangles.size()/3;i++)
		{
			// check if proc owns any nodes in the triangle
			for(int j:range(3))
			{
				int globalId = ugrid.triangles[3*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendTriangleIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendTriangles;
		sendTriangles.reserve(3*sendTriangleIds.size());
		vector<int> sendTriangleTags;
		sendTriangleTags.reserve(sendTriangleIds.size());
		for(int id:sendTriangleIds)
		{
			sendTriangleTags.push_back(ugrid.triangleTags[id]);
			for(int j:range(3))
				sendTriangles.push_back(ugrid.triangles[3*id+j]);	
		}
		vector<int> tmpMap;
		Gatherv(sendTriangles,recvTriangles,tmpMap,proc);
		Gatherv(sendTriangleTags,recvTriangleTags,tmpMap,proc);
	}
}

void ParallelMeshReDistributor::shuffleQuads()
{
	vector<int> sendQuadIds;
	sendQuadIds.reserve(4*ugrid.quads.size());
	for(int proc:range(nproc))
	{
		sendQuadIds.clear();
		vector<int> neededNodeIds;
		if(Rank() == proc)
			neededNodeIds = recvIds;
		Broadcast(neededNodeIds,proc);
		// loop over quads
		for(int i=0;i<ugrid.quads.size()/4;i++)
		{
			// check if proc owns any nodes in the quad
			for(int j:range(4))
			{
				int globalId = ugrid.quads[4*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendQuadIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendQuads;
		sendQuads.reserve(4*sendQuadIds.size());
		vector<int> sendQuadTags;
		sendQuadTags.reserve(sendQuadIds.size());
		for(int id:sendQuadIds)
		{
			sendQuadTags.push_back(ugrid.quadTags[id]);
			for(int j:range(4))
				sendQuads.push_back(ugrid.quads[4*id+j]);	
		}
		vector<int> tmpMap;
		Gatherv(sendQuads,recvQuads,tmpMap,proc);
		Gatherv(sendQuadTags,recvQuadTags,tmpMap,proc);
	}
}

void ParallelMeshReDistributor::shuffleTets()
{
	vector<int> sendTetIds;
	sendTetIds.reserve(4*ugrid.tets.size());
	for(int proc:range(nproc))
	{
		sendTetIds.clear();
		vector<int> neededNodeIds;
		if(Rank() == proc)
			neededNodeIds = recvIds;
		Broadcast(neededNodeIds,proc);
		// loop over tets
		for(int i=0;i<ugrid.tets.size()/4;i++)
		{
			// check if proc owns any nodes in the tet
			for(int j:range(4))
			{
				int globalId = ugrid.tets[4*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendTetIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendTets;
		sendTets.reserve(4*sendTetIds.size());
		for(int id:sendTetIds)
			for(int j:range(4))
				sendTets.push_back(ugrid.tets[4*id+j]);	
		vector<int> tmpMap;
		Gatherv(sendTets,recvTets,tmpMap,proc);
	}
}

void ParallelMeshReDistributor::shufflePyramids()
{
	vector<int> sendPyramidIds;
	sendPyramidIds.reserve(5*ugrid.pyramids.size());
	for(int proc:range(nproc))
	{
		sendPyramidIds.clear();
		vector<int> neededNodeIds;
		if(Rank() == proc)
			neededNodeIds = recvIds;
		Broadcast(neededNodeIds,proc);
		// loop over pyramids
		for(int i=0;i<ugrid.pyramids.size()/5;i++)
		{
			// check if proc owns any nodes in the pyramid
			for(int j:range(5))
			{
				int globalId = ugrid.pyramids[5*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendPyramidIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendPyramids;
		sendPyramids.reserve(5*sendPyramidIds.size());
		for(int id:sendPyramidIds)
			for(int j:range(5))
				sendPyramids.push_back(ugrid.pyramids[5*id+j]);	
		vector<int> tmpMap;
		Gatherv(sendPyramids,recvPyramids,tmpMap,proc);
	}
}

void ParallelMeshReDistributor::shufflePrisms()
{
	vector<int> sendPrismIds;
	sendPrismIds.reserve(6*ugrid.prisms.size());
	for(int proc:range(nproc))
	{
		sendPrismIds.clear();
		vector<int> neededNodeIds;
		if(Rank() == proc)
			neededNodeIds = recvIds;
		Broadcast(neededNodeIds,proc);
		// loop over prisms
		for(int i=0;i<ugrid.prisms.size()/6;i++)
		{
			// check if proc owns any nodes in the prism
			for(int j:range(6))
			{
				int globalId = ugrid.prisms[6*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendPrismIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendPrisms;
		sendPrisms.reserve(6*sendPrismIds.size());
		for(int id:sendPrismIds)
			for(int j:range(6))
				sendPrisms.push_back(ugrid.prisms[6*id+j]);	
		vector<int> tmpMap;
		Gatherv(sendPrisms,recvPrisms,tmpMap,proc);
	}
}

void ParallelMeshReDistributor::shuffleHexs()
{
	vector<int> sendHexIds;
	sendHexIds.reserve(8*ugrid.hexs.size());
	for(int proc:range(nproc))
	{
		sendHexIds.clear();
		vector<int> neededNodeIds;
		if(Rank() == proc)
			neededNodeIds = recvIds;
		Broadcast(neededNodeIds,proc);
		// loop over hexs
		for(int i=0;i<ugrid.hexs.size()/8;i++)
		{
			// check if proc owns any nodes in the hex
			for(int j:range(8))
			{
				int globalId = ugrid.hexs[8*i+j];
				if(binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalId))
				{
					sendHexIds.push_back(i);
					break;
				}
			}
		}
		vector<int> sendHexs;
		sendHexs.reserve(8*sendHexIds.size());
		for(int id:sendHexIds)
			for(int j:range(8))
				sendHexs.push_back(ugrid.hexs[8*id+j]);	
		vector<int> tmpMap;
		Gatherv(sendHexs,recvHexs,tmpMap,proc);
	}
}

int ParallelMeshReDistributor::localId(int globalId)
{
	auto position = std::lower_bound(recvIds.begin(),recvIds.end(),globalId);
	if(position == recvIds.end() or *position != globalId)
	{
		position = std::lower_bound(recvGhostIds.begin(),recvGhostIds.end(),globalId);
		if(position == recvGhostIds.end())
		{
			assert(false);
			return -1;
		}
		return position-recvGhostIds.begin() + recvIds.size();
	}
	else
	{
		return position-recvIds.begin();
	}
}

Fun3DMesh ParallelMeshReDistributor::createFun3DMesh(vector<MapbcReader> &mapbcVector)
{
	// allocate space for Fun3D arrays
	int nnodes0 = recvNodes.size()/3;
	int nnodes01 = nnodes0 + recvGhostIds.size();
	int ntriangles,nquads,ntets,npyramids,nprisms,nhexs;
	ntriangles = recvTriangles.size()/3;
	nquads = recvQuads.size()/4;
	ntets = recvTets.size()/4;
	npyramids = recvPyramids.size()/5;
	nprisms = recvPrisms.size()/6;
	nhexs = recvHexs.size()/8;
	double *x = (double*)malloc(nnodes01*sizeof(double));
	assert(x != NULL);
	double *y = (double*)malloc(nnodes01*sizeof(double));
	assert(y != NULL);
	double *z = (double*)malloc(nnodes01*sizeof(double));
	assert(z != NULL);
	int *globalNodeIds = (int*)malloc(nnodes01*sizeof(int));
	assert(globalNodeIds != NULL);
	int *imesh = (int*)malloc(nnodes01*sizeof(int));
	assert(imesh);
	for(int i:range(nnodes01))
		imesh[i] = -1;
	int *triangles = (int*)malloc(3*ntriangles*sizeof(int));
	assert(triangles != NULL);
	int *triangleTags = (int*)malloc(ntriangles*sizeof(int));
	assert(triangleTags != NULL);
	int *triangleBcs = (int*)malloc(ntriangles*sizeof(int));
	assert(triangleBcs != NULL);
	int *quads = (int*)malloc(4*nquads*sizeof(int));
	assert(quads != NULL);
	int *quadTags = (int*)malloc(nquads*sizeof(int));
	assert(quadTags != NULL);
	int *quadBcs = (int*)malloc(nquads*sizeof(int));
	assert(quadBcs != NULL);
	int *tets = (int*)malloc(4*ntets*sizeof(int));
	assert(tets != NULL);
	int *pyramids = (int*)malloc(5*npyramids*sizeof(int));
	assert(pyramids != NULL);
	int *prisms = (int*)malloc(6*nprisms*sizeof(int));
	assert(prisms != NULL);
	int *hexs = (int*)malloc(8*nhexs*sizeof(int));
	assert(hexs != NULL);
	// copy nodes 
	for(int i:range(nnodes0))
	{
		x[i] = recvNodes[3*i+0];
		y[i] = recvNodes[3*i+1];
		z[i] = recvNodes[3*i+2];
		globalNodeIds[i] = recvIds[i];
		imesh[i] = calcImesh(globalNodeIds[i]);	
	}
	// copy ghost nodes
	for(int i:range(recvGhostIds))
	{
		x[i+nnodes0] = recvGhosts[3*i+0];
		y[i+nnodes0] = recvGhosts[3*i+1];
		z[i+nnodes0] = recvGhosts[3*i+2];
		globalNodeIds[i+nnodes0] = recvGhostIds[i];
		imesh[i+nnodes0] = calcImesh(globalNodeIds[i+nnodes0]);
	}
	// copy cells
	for(int i:range(recvTriangles))
		triangles[i] = localId(recvTriangles[i]);
	for(int i:range(recvTriangleTags))
		triangleTags[i] = recvTriangleTags[i];
	for(int i:range(recvTriangleTags))
	{
		int componentId = imesh[triangles[3*i]];
		triangleBcs[i] = mapbcVector[componentId].boundaryCondition(triangleTags[i]);
	}
	for(int i:range(recvQuads))
		quads[i] = localId(recvQuads[i]);
	for(int i:range(recvQuadTags))
		quadTags[i] = recvQuadTags[i];
	for(int i:range(recvQuadTags))
	{
		int componentId = imesh[quads[4*i]];
		quadBcs[i] = mapbcVector[componentId].boundaryCondition(quadTags[i]);
	}

	for(int i:range(recvTets))
		tets[i] = localId(recvTets[i]);
	for(int i:range(recvPyramids))
		pyramids[i] = localId(recvPyramids[i]);
	for(int i:range(recvPrisms))
		prisms[i] = localId(recvPrisms[i]);
	for(int i:range(recvHexs))
		hexs[i] = localId(recvHexs[i]);


	Fun3DMesh funMesh(nnodes0,nnodes01,x,y,z,
			globalNodeIds,
			imesh,
			ntriangles,triangles,triangleTags,triangleBcs,
			nquads,quads,quadTags,quadBcs,ntets,tets,
			npyramids,pyramids,
			nprisms,prisms,
			nhexs,hexs);
	return funMesh;
}

int ParallelMeshReDistributor::calcImesh(int globalId)
{
	for(int i=1;i<gridNodeMap.size();i++)
		if(globalId < gridNodeMap[i])
			return i-1;
	printf("globalId: %i, max: %i\n",globalId,gridNodeMap.size());
	assert(false);
}

#endif
