#include "parallel_mesh_importer.h"

using namespace UgridReader;
using namespace MessagePasser;
using std::vector;

void ParallelMeshImporter::distributeNodes()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myNodes = getNodes(0,procNodeMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			vector<double> nodeBuffer = getNodes(procNodeMap[proc],procNodeMap[proc+1]);
			Send(nodeBuffer,proc);
		}
	}
	else
		Recv(myNodes,0);
}

void ParallelMeshImporter::distributeTriangles()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myTriangles = getTriangles(0,procTriangleMap[1]);
		myTriangleTags = getTriangleTags(0,procTriangleMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			vector<int> triangleBuffer = getTriangles(procTriangleMap[proc],procTriangleMap[proc+1]);
			vector<int> triangleTagBuffer = getTriangleTags(procTriangleMap[proc],procTriangleMap[proc+1]);
			Send(triangleBuffer,proc);
			Send(triangleTagBuffer,proc);
		}
	}
	else
	{
		Recv(myTriangles,0);
		Recv(myTriangleTags,0);
	}
}

void ParallelMeshImporter::distributeQuads()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myQuads = getQuads(0,procQuadMap[1]);
		myQuadTags = getQuadTags(0,procQuadMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			vector<int> quadBuffer = getQuads(procQuadMap[proc],procQuadMap[proc+1]);
			vector<int> quadTagBuffer = getQuadTags(procQuadMap[proc],procQuadMap[proc+1]);
			Send(quadBuffer,proc);
			Send(quadTagBuffer,proc);
		}
	}
	else
	{
		Recv(myQuads,0);
		Recv(myQuadTags,0);
	}
}

void ParallelMeshImporter::distributeTets()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myTets = getTets(0,procTetMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			std::vector<int> tetBuffer = getTets(procTetMap[proc],procTetMap[proc+1]);
			Send(tetBuffer,proc);
		}
	}
	else
		Recv(myTets,0);
}

void ParallelMeshImporter::distributePyramids()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myPyramids = getPyramids(0,procPyramidMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			std::vector<int> pyramidBuffer = getPyramids(procPyramidMap[proc],procPyramidMap[proc+1]);
			Send(pyramidBuffer,proc);
		}
	}
	else
		Recv(myPyramids,0);
}

void ParallelMeshImporter::distributePrisms()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myPrisms = getPrisms(0,procPrismMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			std::vector<int> prismBuffer = getPrisms(procPrismMap[proc],procPrismMap[proc+1]);
			Send(prismBuffer,proc);
		}
	}
	else
		Recv(myPrisms,0);
}

void ParallelMeshImporter::distributeHexs()
{
	if(Rank() == 0)
	{
		int nproc = NumberOfProcesses();
		myHexs = getHexs(0,procHexMap[1]);
		for(int proc=1;proc<nproc;proc++)
		{
			std::vector<int> hexBuffer = getHexs(procHexMap[proc],procHexMap[proc+1]);
			Send(hexBuffer,proc);
		}
	}
	else
		Recv(myHexs,0);
}

std::vector<double> ParallelMeshImporter::getNodes(int begin,int end)
{
	//printf("Grid node map ");
	//for(int count:gridNodeMap)
	//	printf("%i ",count);
	//printf("\n");
	//printf("Reading nodes %i - %i\n",begin,end);
	std::vector<double> nodeBuffer(3*(end-begin),0.0);
	int firstGrid  = getFirstGrid(gridNodeMap,begin);
	int lastGrid   = getLastGrid(gridNodeMap,end);
	int beginIndex = getBeginIndex(gridNodeMap,begin);
	int endIndex   = getEndIndex(gridNodeMap,end);
	int positionInBuffer = 0;
	std::vector<double> tmp;
	if(firstGrid == lastGrid)
	{
		//printf("--From grid %i: reading %i to %i\n",firstGrid,beginIndex,endIndex);
		// read nodes from the first grid (start at beginIndex and read to endIndex)
		tmp = readNodes(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		//printf("--From grid %i: reading %i to %i\n",firstGrid,beginIndex,gridNodeMap[firstGrid+1]);
		// read nodes from the first grid (start at beginIndex and read to the end of the file)
		tmp = readNodes(gridFiles[firstGrid],beginIndex,
				gridNodeMap[firstGrid+1]-gridNodeMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(double node : tmp) 
		nodeBuffer[positionInBuffer++] = node;   
	tmp.clear();

	// read all nodes from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		//printf("--From grid %i: reading all nodes\n",i);
		tmp = readNodes(gridFiles[i],isBigEndian[i]);
		for(double node : tmp)
			nodeBuffer[positionInBuffer++] = node;
		tmp.clear();
	}
	// read nodes from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp.clear();
		//printf("--From grid %i: reading nodes 0 - %i\n",lastGrid,endIndex);
		tmp = readNodes(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(double node : tmp)
			nodeBuffer[positionInBuffer++] = node;
	}

	return nodeBuffer;
}

std::vector<int> ParallelMeshImporter::getTriangles(int begin,int end)
{
	std::vector<int> triangleBuffer(3*(end-begin),0.0);
	int firstGrid  = getFirstGrid(gridTriangleMap,begin);
	int lastGrid   = getLastGrid(gridTriangleMap,end);
	int beginIndex = getBeginIndex(gridTriangleMap,begin);
	int endIndex   = getEndIndex(gridTriangleMap,end);
	int positionInBuffer = 0;
	std::vector<int> tmp;
	if(firstGrid == lastGrid)
	{
		// read triangles from the first grid (start at beginIndex and read to endIndex)
		tmp = readTriangles(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read triangles from the first grid (start at beginIndex and read to the end of the file)
		tmp = readTriangles(gridFiles[firstGrid],beginIndex,gridTriangleMap[firstGrid+1]
				-gridTriangleMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int triangle : tmp) 
		triangleBuffer[positionInBuffer++] = triangle + gridNodeMap[firstGrid];   
	tmp.clear();
	
	// read all triangles from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readTriangles(gridFiles[i],isBigEndian[i]);
		for(int triangle : tmp)
			triangleBuffer[positionInBuffer++] = triangle + gridNodeMap[i];
		tmp.clear();
	}
	
	// read triangles from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readTriangles(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int triangle : tmp)
			triangleBuffer[positionInBuffer++] = triangle + gridNodeMap[lastGrid];
	}
	return triangleBuffer;
}

vector<int> ParallelMeshImporter::getTriangleTags(int begin,int end)
{
	std::vector<int> triangleTagBuffer(end-begin,0);
	int firstGrid  = getFirstGrid(gridTriangleMap,begin);
	int lastGrid   = getLastGrid(gridTriangleMap,end);
	int beginIndex = getBeginIndex(gridTriangleMap,begin);
	int endIndex   = getEndIndex(gridTriangleMap,end);
	int positionInBuffer = 0;
	vector<int> tmp;
	if(firstGrid == lastGrid)
	{
		// read triangles from the first grid (start at beginIndex and read to endIndex)
		tmp = readTriangleBoundaryTags(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read triangles from the first grid (start at beginIndex and read to the end of the file)
		tmp = readTriangleBoundaryTags(gridFiles[firstGrid],beginIndex,gridTriangleMap[firstGrid+1]
				-gridTriangleMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int tag : tmp) 
		triangleTagBuffer[positionInBuffer++] = tag;   
	tmp.clear();
	
	// read all triangles from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readTriangleBoundaryTags(gridFiles[i],isBigEndian[i]);
		for(int tag : tmp)
			triangleTagBuffer[positionInBuffer++] = tag;
		tmp.clear();
	}
	
	// read triangles from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readTriangleBoundaryTags(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int tag : tmp)
			triangleTagBuffer[positionInBuffer++] = tag;
	}
	return triangleTagBuffer;
}

std::vector<int> ParallelMeshImporter::getQuads(int begin,int end)
{
	std::vector<int> quadBuffer(4*(end-begin),0.0);
	int firstGrid  = getFirstGrid(gridQuadMap,begin);
	int lastGrid   = getLastGrid(gridQuadMap,end);
	int beginIndex = getBeginIndex(gridQuadMap,begin);
	int endIndex   = getEndIndex(gridQuadMap,end);
	int positionInBuffer = 0;
	std::vector<int> tmp;	
	if(firstGrid == lastGrid)
	{
		// read quads from the first grid (start at beginIndex and read to endIndex)
		tmp = readQuads(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read quads from the first grid (start at beginIndex and read to the end of the file)
		tmp = readQuads(gridFiles[firstGrid],beginIndex,gridQuadMap[firstGrid+1]
				-gridQuadMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int quad : tmp) 
		quadBuffer[positionInBuffer++] = quad + gridNodeMap[firstGrid];   
	tmp.clear();
	
	// read all quads from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readQuads(gridFiles[i],isBigEndian[i]);
		for(int quad : tmp)
			quadBuffer[positionInBuffer++] = quad + gridNodeMap[i];
		tmp.clear();
	}
	
	// read quads from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readQuads(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int quad : tmp)
			quadBuffer[positionInBuffer++] = quad + gridNodeMap[lastGrid];
	}
	return quadBuffer;
}

vector<int> ParallelMeshImporter::getQuadTags(int begin,int end)
{
	std::vector<int> quadTagBuffer(end-begin,0);
	int firstGrid  = getFirstGrid(gridQuadMap,begin);
	int lastGrid   = getLastGrid(gridQuadMap,end);
	int beginIndex = getBeginIndex(gridQuadMap,begin);
	int endIndex   = getEndIndex(gridQuadMap,end);
	int positionInBuffer = 0;
	vector<int> tmp;
	if(firstGrid == lastGrid)
	{
		// read quads from the first grid (start at beginIndex and read to endIndex)
		tmp = readQuadBoundaryTags(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read quads from the first grid (start at beginIndex and read to the end of the file)
		tmp = readQuadBoundaryTags(gridFiles[firstGrid],beginIndex,gridQuadMap[firstGrid+1]
				-gridQuadMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int tag : tmp) 
		quadTagBuffer[positionInBuffer++] = tag;   
	tmp.clear();
	
	// read all quads from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readQuadBoundaryTags(gridFiles[i],isBigEndian[i]);
		for(int tag : tmp)
			quadTagBuffer[positionInBuffer++] = tag;
		tmp.clear();
	}
	
	// read quads from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readQuadBoundaryTags(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int tag : tmp)
			quadTagBuffer[positionInBuffer++] = tag;
	}
	return quadTagBuffer;
}

std::vector<int> ParallelMeshImporter::getTets(int begin,int end)
{
	std::vector<int> tetBuffer(4*(end-begin),0);
	int firstGrid  = getFirstGrid(gridTetMap,begin);
	int lastGrid   = getLastGrid(gridTetMap,end);
	int beginIndex = getBeginIndex(gridTetMap,begin);
	int endIndex   = getEndIndex(gridTetMap,end);
	int positionInBuffer = 0;
	std::vector<int> tmp;	
	if(firstGrid == lastGrid)
	{
		// read tets from the first grid (start at beginIndex and read to endIndex)
		tmp = readTets(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read tets from the first grid (start at beginIndex and read to the end of the file)
		tmp = readTets(gridFiles[firstGrid],beginIndex,gridTetMap[firstGrid+1]
				-gridTetMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int tet : tmp) 
		tetBuffer[positionInBuffer++] = tet + gridNodeMap[firstGrid];   
	tmp.clear();
	// read all tets from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readTets(gridFiles[i],isBigEndian[i]);
		for(int tet : tmp)
			tetBuffer[positionInBuffer++] = tet + gridNodeMap[i];
		tmp.clear();
	}
	
	// read tets from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readTets(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int tet : tmp)
			tetBuffer[positionInBuffer++] = tet + gridNodeMap[lastGrid];
	}	
	return tetBuffer;
}

std::vector<int> ParallelMeshImporter::getPyramids(int begin,int end)
{
	std::vector<int> pyramidBuffer(5*(end-begin),0.0);
	int firstGrid  = getFirstGrid(gridPyramidMap,begin);
	int lastGrid   = getLastGrid(gridPyramidMap,end);
	int beginIndex = getBeginIndex(gridPyramidMap,begin);
	int endIndex   = getEndIndex(gridPyramidMap,end);
	int positionInBuffer = 0;
	std::vector<int> tmp;	
	if(firstGrid == lastGrid)
	{
		// read pyramids from the first grid (start at beginIndex and read endIndex)
		tmp = readPyramids(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read pyramids from the first grid (start at beginIndex and read to the end of the file)
		tmp = readPyramids(gridFiles[firstGrid],beginIndex,gridPyramidMap[firstGrid+1]
				-gridPyramidMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int pyramid : tmp) 
		pyramidBuffer[positionInBuffer++] = pyramid + gridNodeMap[firstGrid];   
	tmp.clear();
	
	// read all pyramids from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readPyramids(gridFiles[i],isBigEndian[i]);
		for(int pyramid : tmp)
			pyramidBuffer[positionInBuffer++] = pyramid + gridNodeMap[i];
		tmp.clear();
	}
	
	// read pyramids from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readPyramids(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int pyramid : tmp)
			pyramidBuffer[positionInBuffer++] = pyramid + gridNodeMap[lastGrid];
	}	
	return pyramidBuffer;
}

std::vector<int> ParallelMeshImporter::getPrisms(int begin,int end)
{
	std::vector<int> prismBuffer(6*(end-begin),0.0);
	int firstGrid  = getFirstGrid(gridPrismMap,begin);
	int lastGrid   = getLastGrid(gridPrismMap,end);
	int beginIndex = getBeginIndex(gridPrismMap,begin);
	int endIndex   = getEndIndex(gridPrismMap,end);
	int positionInBuffer = 0;

	std::vector<int> tmp;
	if(firstGrid == lastGrid)
	{
		// read prisms from the first grid (start at beginIndex and read to endIndex)
		tmp = readPrisms(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read prisms from the first grid (start at beginIndex and read to the end of the file)
		tmp = readPrisms(gridFiles[firstGrid],beginIndex,gridPrismMap[firstGrid+1]
				-gridPrismMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int prism : tmp) 
		prismBuffer[positionInBuffer++] = prism + gridNodeMap[firstGrid];   
	tmp.clear();
	
	// read all prisms from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readPrisms(gridFiles[i],isBigEndian[i]);
		for(int prism : tmp)
			prismBuffer[positionInBuffer++] = prism + gridNodeMap[i];
		tmp.clear();
	}
	
	// read prisms from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readPrisms(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int prism : tmp)
			prismBuffer[positionInBuffer++] = prism + gridNodeMap[lastGrid];
	}	
	return prismBuffer;
}

std::vector<int> ParallelMeshImporter::getHexs(int begin,int end)
{
	std::vector<int> hexBuffer(8*(end-begin),0.0);
	int firstGrid  = getFirstGrid(gridHexMap,begin);
	int lastGrid   = getLastGrid(gridHexMap,end);
	int beginIndex = getBeginIndex(gridHexMap,begin);
	int endIndex   = getEndIndex(gridHexMap,end);
	int positionInBuffer = 0;
	std::vector<int> tmp;
	if(firstGrid == lastGrid)
	{
		// read hexs from the first grid (start at beginIndex and read to endIndex)
		tmp = readHexs(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);	
	}
	else
	{
		// read hexs from the first grid (start at beginIndex and read to the end of the file)
		tmp = readHexs(gridFiles[firstGrid],beginIndex,gridHexMap[firstGrid+1]
				-gridHexMap[firstGrid],isBigEndian[firstGrid]);	
	}
	for(int hex : tmp) 
		hexBuffer[positionInBuffer++] = hex + gridNodeMap[firstGrid];   
	tmp.clear();
	
	// read all hexs from grids between first and last grid
	for(int i=firstGrid+1;i<lastGrid;i++)
	{
		tmp = readHexs(gridFiles[i],isBigEndian[i]);
		for(int hex : tmp)
			hexBuffer[positionInBuffer++] = hex + gridNodeMap[i];
		tmp.clear();
	}
	
	// read hexs from last grid (start at zero and end at endIndex)
	if(lastGrid > firstGrid)
	{
		tmp = readHexs(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
		for(int hex : tmp)
			hexBuffer[positionInBuffer++] = hex + gridNodeMap[lastGrid];
	}
	return hexBuffer;
}

int ParallelMeshImporter::getFirstGrid(std::vector<int> &gridMap,int begin)
{
	int ngrid = (int)gridFiles.size();
	for(int i=0;i<ngrid;i++)
		if(begin <= gridMap[i+1])
			return i;
	assert(false);
}

int ParallelMeshImporter::getLastGrid(std::vector<int> &gridMap,int end)
{
	if(0==end){return 0;}
	int ngrid = (int)gridFiles.size();
	for(int i=ngrid-1;i>=0;i--)
		if(end >= gridMap[i])
			return i;
	assert(false);
}

int ParallelMeshImporter::getBeginIndex(std::vector<int> &gridMap,int begin)
{
	int ngrid = (int)gridFiles.size();
	for(int i=0;i<ngrid;i++)
	{
		if(begin <= gridMap[i+1])
			return begin-gridMap[i];
	}
	assert(false);
}

int ParallelMeshImporter::getEndIndex(std::vector<int> &gridMap,int end)
{
	if(0==end){return 0;}
	int ngrid = (int)gridFiles.size();
	for(int i=ngrid-1;i>=0;i--)
		if(end >= gridMap[i])
			return end-gridMap[i];
	assert(false);
}
