#include "parallel_mesh_reader_naive.h"
#include "range_loop.h"
#include <stdio.h>

using namespace UgridReader;
using namespace MessagePasser;

ParallelMeshReaderNaive::ParallelMeshReaderNaive(std::vector<std::string> gridFiles_in,vector<bool> &isBigEndian_in)
	:isBigEndian(isBigEndian_in)
{

	gridFiles = gridFiles_in;
	int ngrid = (int)gridFiles.size();
	gridNodeMap.reserve(ngrid);
	gridTriangleMap.reserve(ngrid);
	gridQuadMap.reserve(ngrid);
	gridTetMap.reserve(ngrid);
	gridPyramidMap.reserve(ngrid);
	gridPrismMap.reserve(ngrid);
	gridHexMap.reserve(ngrid);
	gridNodeMap.push_back(0);
	gridTriangleMap.push_back(0);
	gridQuadMap.push_back(0);
	gridTetMap.push_back(0);
	gridPyramidMap.push_back(0);
	gridPrismMap.push_back(0);
	gridHexMap.push_back(0);
	for(int i:range(gridFiles))
	{
		int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;	
		readHeader(gridFiles[i],nnodes,ntri,nquad,ntet,npyr,nprism,nhex,isBigEndian[i]);
		gridNodeMap.push_back(nnodes);
		gridTriangleMap.push_back(ntri);
		gridQuadMap.push_back(nquad);
		gridTetMap.push_back(ntet);
		gridPyramidMap.push_back(npyr);
		gridPrismMap.push_back(nprism);
		gridHexMap.push_back(nhex);
	}

	for(int i=2;i<(int)gridNodeMap.size();i++)
	{
		gridNodeMap[i]     += gridNodeMap[i-1];
		gridTriangleMap[i] += gridTriangleMap[i-1];
		gridQuadMap[i]     += gridQuadMap[i-1];
		gridTetMap[i]      += gridTetMap[i-1];
		gridPyramidMap[i]  += gridPyramidMap[i-1];
		gridPrismMap[i]    += gridPrismMap[i-1];
		gridHexMap[i]      += gridHexMap[i-1];
	}
	if(Rank() == 0)
	{
		printf("Composite domain has:\n");
		printf("--nnodes        %i\n",gridNodeMap.back());
		printf("--triangles     %i\n",gridTriangleMap.back());
		printf("--quads         %i\n",gridQuadMap.back());
		printf("--tets          %i\n",gridTetMap.back());
		printf("--pyramids      %i\n",gridPyramidMap.back());
		printf("--prisms        %i\n",gridPrismMap.back());
		printf("--hexs          %i\n",gridHexMap.back());
	}
	distributeUgrid();
}

ParallelMeshReaderNaive::~ParallelMeshReaderNaive()
{
}

vector<int> ParallelMeshReaderNaive::getGridNodeMap()
{
	return gridNodeMap;
}

vector<int> ParallelMeshReaderNaive::getProcNodeMap()
{
	return procCellMap;
}

vector<int> ParallelMeshReaderNaive::getProcCellMap()
{
	std::vector<int> procCellMap;
	int ncells = myTets.size()/4 + myPyramids.size()/5 + myPrisms.size()/6 + myHexs.size()/8;
	AllGather(ncells,procCellMap);
	procCellMap.insert(procCellMap.begin(),0);
	for(int i=1;i<procCellMap.size();i++)
		procCellMap[i] += procCellMap[i-1];
	return procCellMap;
}

void ParallelMeshReaderNaive::distributeUgrid()
{
	if(0 == Rank())
		buildDistributionMaps();
	Broadcast(procNodeMap,0);
	Broadcast(procTriangleMap,0);
	Broadcast(procQuadMap,0);
	Broadcast(procTetMap,0);
	Broadcast(procPyramidMap,0);
	Broadcast(procPrismMap,0);
	Broadcast(procHexMap,0);
	
	if(Rank() == 0)
		printf("Distributing ...\n");
	distributeNodes();
	distributeTriangles();
	distributeQuads();
	distributeTets();
	distributePyramids();
	distributePrisms();
	distributeHexs();
	
	if(Rank() == 0)
		printf("Done Distributing ...\n");
}

void ParallelMeshReaderNaive::buildDistributionMaps()
{
	// count up the total number of nodes/elements in all component grids
	int totalNodes=0,totalTriangles=0,totalQuads=0,totalTets=0,
		totalPyramids=0,totalPrisms=0,totalHexs=0;
	for(int i:range(gridFiles))
	{
		int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;	
		readHeader(gridFiles[i],nnodes,ntri,nquad,ntet,npyr,nprism,nhex,isBigEndian[i]);
		totalNodes      += nnodes;
		totalTriangles  += ntri;
		totalQuads      += nquad;
		totalTets       += ntet;
		totalPyramids   += npyr;
		totalPrisms     += nprism;
		totalHexs       += nhex;
	}
	int nproc = NumberOfProcesses();
	procNodeMap.assign(nproc,0);
	procTriangleMap.assign(nproc,0);
	procQuadMap.assign(nproc,0);
	procTetMap.assign(nproc,0);
	procPyramidMap.assign(nproc,0);
	procPrismMap.assign(nproc,0);
	procHexMap.assign(nproc,0);
	

	// count how many nodes/elements go to each proc
	for(int i=0;i<nproc;i++)
	{
		procNodeMap[i]     = totalNodes/nproc;
		procTriangleMap[i] = totalTriangles/nproc;
		procQuadMap[i]     = totalQuads/nproc;
		procTetMap[i]      = totalTets/nproc;
		procPyramidMap[i]  = totalPyramids/nproc;
		procPrismMap[i]    = totalPrisms/nproc;
		procHexMap[i]      = totalHexs/nproc;
	}
	for(int i=0;i<totalNodes%nproc;i++)
		procNodeMap[i]++;
	for(int i=0;i<totalTriangles%nproc;i++)
		procTriangleMap[i]++;
	for(int i=0;i<totalQuads%nproc;i++)
		procQuadMap[i]++;
	for(int i=0;i<totalTets%nproc;i++)
		procTetMap[i]++;
	for(int i=0;i<totalPyramids%nproc;i++)
		procPyramidMap[i]++;
	for(int i=0;i<totalPrisms%nproc;i++)
		procPrismMap[i]++;
	for(int i=0;i<totalHexs%nproc;i++)
		procHexMap[i]++;
	// convert to a map
	procNodeMap.insert(procNodeMap.begin(),0);
	procTriangleMap.insert(procTriangleMap.begin(),0);
	procQuadMap.insert(procQuadMap.begin(),0);
	procTetMap.insert(procTetMap.begin(),0);
	procPyramidMap.insert(procPyramidMap.begin(),0);
	procPrismMap.insert(procPrismMap.begin(),0);
	procHexMap.insert(procHexMap.begin(),0);
	for(int i=2;i<nproc+1;i++)
	{
		procNodeMap[i]     += procNodeMap[i-1];
		procTriangleMap[i] += procTriangleMap[i-1];
		procQuadMap[i]     += procQuadMap[i-1];
		procTetMap[i]      += procTetMap[i-1];
		procPyramidMap[i]  += procPyramidMap[i-1];
		procPrismMap[i]    += procPrismMap[i-1];
		procHexMap[i]      += procHexMap[i-1];
	}
}

ImportedUgrid ParallelMeshReaderNaive::distributeGridsEvenly()
{
	ImportedUgrid ugrid(myNodes,myTriangles,myQuads,myTets,
			myPyramids,myPrisms,myHexs,myTriangleTags,myQuadTags,
			myTriangleTags,myQuadTags);
	return ugrid;
}
