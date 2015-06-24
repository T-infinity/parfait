#include "byteswap.h"
#include "message_passer.h"
#include <stdexcept>
#include <vector>

using namespace Parfait;

inline void UgridWriter::writeImportedUgrid(ImportedUgrid &ugrid, std::string filename, bool swapBytes)
{
	using namespace MessagePasser;
	int numberOfNodes      = ParallelSum(ugrid.numberOfNodes(),0);
	int numberOfTets       = ParallelSum(ugrid.tets.size()/4,0);
   	int numberOfPyramids   = ParallelSum(ugrid.pyramids.size()/5,0);
	int numberOfPrisms     = ParallelSum(ugrid.prisms.size()/6,0);
	int numberOfHexs       = ParallelSum(ugrid.hexs.size()/8,0);
	int numberOfTriangles  = ParallelSum(ugrid.triangles.size()/3,0);
	int numberOfQuads      = ParallelSum(ugrid.quads.size()/4,0);

	if(Rank() == 0)
	{
    	writeHeader(filename, numberOfNodes, numberOfTriangles, 
            numberOfQuads, numberOfTets, numberOfPyramids, 
			numberOfPrisms, numberOfHexs, swapBytes);
	}

	// transfer nodes to root proc and write out in order
	if(Rank() == 0)
	{
    	writeNodes(filename, ugrid.nodes.size()/3, ugrid.nodes.data(), swapBytes);
		std::vector<double> recvNodes;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvNodes,proc);
    		writeNodes(filename, recvNodes.size()/3, recvNodes.data(), swapBytes);
		}
	}
	else
		Send(ugrid.nodes,0);

	// transfer triangles to root proc and write out in order
	if(Rank() == 0)
	{
    	writeTriangles(filename, ugrid.triangles.size() / 3, ugrid.triangles.data(), swapBytes);
		std::vector<int> recvTriangles;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvTriangles,proc);
			writeTriangles(filename, recvTriangles.size() / 3, recvTriangles.data(), swapBytes);
		}
	}
	else
		Send(ugrid.triangles,0);

	// transfer quads to root proc and write out in order
	if(Rank() == 0)
	{
    	writeQuads(filename, ugrid.quads.size() / 4, ugrid.quads.data(), swapBytes);
		std::vector<int> recvQuads;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvQuads,proc);
    		writeQuads(filename, recvQuads.size()/4, recvQuads.data(), swapBytes);
		}
	}
	else
		Send(ugrid.quads,0);

	// transfer triangle boundary tags to root and write out in order
	if(Rank() == 0)
	{
    	writeTriangleBoundaryTags(filename, ugrid.triangles.size() / 3, 
							ugrid.triangleTags.data(), swapBytes);
		std::vector<int> recvTriangleTags;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvTriangleTags,proc);
    		writeTriangleBoundaryTags(filename, recvTriangleTags.size(), 
							recvTriangleTags.data(), swapBytes);
		}
	}
	else
		Send(ugrid.triangleTags,0);

	// transfer quad boundary tags to root and write out in order
	if(Rank() == 0)
	{
    	writeQuadBoundaryTags(filename, ugrid.quads.size() / 4, ugrid.quadTags.data(), swapBytes);
		std::vector<int> recvQuadTags;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvQuadTags,proc);
    		writeQuadBoundaryTags(filename, recvQuadTags.size(), recvQuadTags.data(), swapBytes);
		}
	}
	else
		Send(ugrid.quadTags,0);

	// transfer tets to root and write out in order
	if(Rank() == 0)
	{
   	 	writeTets(filename, ugrid.tets.size() / 4, ugrid.tets.data(), swapBytes);
		std::vector<int> recvTets;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvTets,proc);
   	 		writeTets(filename, recvTets.size() / 4, recvTets.data(), swapBytes);
		}
	}
	else
		Send(ugrid.tets,0);

	// transfer pyramids to root and write out in order
	if(Rank() == 0)
	{
    	writePyramids(filename, ugrid.pyramids.size() / 5, ugrid.pyramids.data(), swapBytes);
		std::vector<int> recvPyramids;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvPyramids,proc);
    		writePyramids(filename, recvPyramids.size() / 5, recvPyramids.data(), swapBytes);
		}
	}
	else
		Send(ugrid.pyramids,0);

	// transfer prisms to root and write out in order
	if(Rank() == 0)
	{
    	writePrisms(filename, ugrid.prisms.size() / 6, ugrid.prisms.data(), swapBytes);
		std::vector<int> recvPrisms;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvPrisms,proc);
    		writePrisms(filename, recvPrisms.size() / 6, recvPrisms.data(), swapBytes);
		}
	}
	else
		Send(ugrid.prisms,0);

	// send hexs to root and write out in order
	if(Rank() == 0)
	{
    	writeHexs(filename, ugrid.hexs.size() / 8, ugrid.hexs.data(), swapBytes);
		std::vector<int> recvHexs;
		for(int proc=1;proc<NumberOfProcesses();proc++)
		{
			Recv(recvHexs,proc);
    		writeHexs(filename, recvHexs.size() / 8, recvHexs.data(), swapBytes);
		}
	}
	else
		Send(ugrid.hexs,0);
}

inline void UgridWriter::writeHeader(std::string filename,int nnodes,
        int ntri,int nquad,
        int ntet,int npyr,int nprism,int nhex,bool swapBytes)
{
    FILE *f = fopen(filename.c_str(),"wb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
	if(swapBytes)
	{
		private_fwrite(&nnodes,sizeof(int),1,f);
		private_fwrite(&ntri  ,sizeof(int),1,f);
		private_fwrite(&nquad ,sizeof(int),1,f);
		private_fwrite(&ntet  ,sizeof(int),1,f);
		private_fwrite(&npyr  ,sizeof(int),1,f);
		private_fwrite(&nprism,sizeof(int),1,f);
		private_fwrite(&nhex  ,sizeof(int),1,f);
	}
	else
	{
		fwrite(&nnodes,sizeof(int),1,f);
		fwrite(&ntri  ,sizeof(int),1,f);
		fwrite(&nquad ,sizeof(int),1,f);
		fwrite(&ntet  ,sizeof(int),1,f);
		fwrite(&npyr  ,sizeof(int),1,f);
		fwrite(&nprism,sizeof(int),1,f);
		fwrite(&nhex  ,sizeof(int),1,f);
	}
    fclose(f);
}

inline void UgridWriter::writeNodes(std::string filename,int nnodes,double *nodes,bool swapBytes)
{
	FILE *f = fopen(filename.c_str(),"ab");
	assert(f != NULL);
	if(swapBytes)
		private_fwrite(nodes,sizeof(double),3*nnodes,f);
	else
		fwrite(nodes,sizeof(double),3*nnodes,f);
	fclose(f);
}

inline void UgridWriter::writeTriangles(std::string filename,int ntriangles,int *triangles,bool swapBytes)
{
	std::vector<int> tris(3*ntriangles,0);
	for(int i=0;i<3*ntriangles;i++) tris[i] = triangles[i]+1;
	writeIntegerField(filename,3*ntriangles,tris.data(),swapBytes);
}

inline void UgridWriter::writeQuads(std::string filename,int nquads,int *quads,bool swapBytes)
{
	std::vector<int> newQuads(4*nquads,0);
	for(int i=0;i<4*nquads;i++) newQuads[i] = quads[i]+1;
	writeIntegerField(filename,4*nquads,newQuads.data(),swapBytes);
}

inline void UgridWriter::writeTets(std::string filename,int ntets,int *tets,bool swapBytes)
{
	std::vector<int> newTets(4*ntets,0);
	for(int i=0;i<4*ntets;i++) newTets[i] = tets[i]+1;
	writeIntegerField(filename,4*ntets,newTets.data(),swapBytes);
}

inline void UgridWriter::writePyramids(std::string filename,int npyramids,int *pyramids,bool swapBytes)
{
	std::vector<int> newPyramids(5*npyramids,0);
	for(int i=0;i<5*npyramids;i++) newPyramids[i] = pyramids[i]+1;
	writeIntegerField(filename,5*npyramids,newPyramids.data(),swapBytes);
}

inline void UgridWriter::writePrisms(std::string filename,int nprisms,int *prisms,bool swapBytes)
{
	std::vector<int> newPrisms(6*nprisms,0);
	for(int i=0;i<6*nprisms;i++) newPrisms[i] = prisms[i]+1;
	writeIntegerField(filename,6*nprisms,newPrisms.data(),swapBytes);
}

inline void UgridWriter::writeHexs(std::string filename,int nhexs,int *hexs,bool swapBytes)
{
	std::vector<int> newHexs(8*nhexs,0);
	for(int i=0;i<8*nhexs;i++) newHexs[i] = hexs[i]+1;
	writeIntegerField(filename,8*nhexs,newHexs.data(),swapBytes);
}

inline void UgridWriter::writeTriangleBoundaryTags(std::string filename,int ntriangles,int *triangleTags,bool swapBytes)
{
	writeIntegerField(filename,ntriangles,triangleTags,swapBytes);
}

inline void UgridWriter::writeQuadBoundaryTags(std::string filename,int nquads,int *quadTags,bool swapBytes)
{
	writeIntegerField(filename,nquads,quadTags,swapBytes);
}


inline void UgridWriter::writeIntegerField(std::string filename,int n,int *fieldData,bool swapBytes)
{
	FILE *f = fopen(filename.c_str(),"ab");
	assert(f != NULL);
	if(swapBytes)
		private_fwrite(fieldData,sizeof(int),n,f);
	else
		fwrite(fieldData,sizeof(int),n,f);
	fclose(f);
}

inline void UgridWriterFactory::setName(std::string fileNameBase) {
    fileName = fileNameBase + ".ugrid";
}

inline void UgridWriterFactory::setNodes(double *nodes_in, int numberOfNodes_in) {
    nodes = nodes_in;
    numberOfNodes = numberOfNodes_in;
}

inline void UgridWriterFactory::setTets(int *tets_in, int numberOfTets_in) {
    tets = tets_in;
    numberOfTets = numberOfTets_in;
}

inline void UgridWriterFactory::setHexes(int *hexes_in, int numberOfHexes_in) {
    hexes = hexes_in;
    numberOfHexes = numberOfHexes_in;
}

inline void UgridWriterFactory::setPyramids(int *pyramids_in, int numberOfPyramids_in) {
    pyramids = pyramids_in;
    numberOfPyramids = numberOfPyramids_in;
}

inline void UgridWriterFactory::setPrisms(int *prisms_in, int numberOfPrisms_in) {
    prisms = prisms_in;
    numberOfPrisms = numberOfPrisms_in;
}

inline void UgridWriterFactory::setTriangles(int *triangles_in, int numberOfTriangles_in) {
    triangles = triangles_in;
    numberOfTriangles = numberOfTriangles_in;
}

inline void UgridWriterFactory::setQuads(int *quads_in, int numberOfQuads_in) {
    quads = quads_in;
    numberOfQuads = numberOfQuads_in;
}

inline void UgridWriterFactory::setTriangleTags(int *triangleTags_in) {
    triangleTags = triangleTags_in;
}

inline void UgridWriterFactory::setQuadTags(int *quadTags_in) {
    quadTags = quadTags_in;
}

inline void UgridWriterFactory::writeFile() {

    UgridWriter::writeHeader(fileName,
            numberOfNodes,
            numberOfTriangles,
            numberOfQuads,
            numberOfTets,
            numberOfPyramids,
            numberOfPrisms,
            numberOfHexes,
            doByteSwap);
    UgridWriter::writeNodes(fileName, numberOfNodes, nodes, doByteSwap);
    UgridWriter::writeTriangles(fileName, numberOfTriangles, triangles, doByteSwap);
    UgridWriter::writeQuads(fileName, numberOfQuads, quads, doByteSwap);
    UgridWriter::writeTriangleBoundaryTags(fileName, numberOfTriangles, triangleTags, doByteSwap);
    UgridWriter::writeQuadBoundaryTags(fileName, numberOfQuads, quadTags, doByteSwap);
    UgridWriter::writeTets(fileName, numberOfTets, tets, doByteSwap);
    UgridWriter::writePyramids(fileName, numberOfPyramids, pyramids, doByteSwap);
    UgridWriter::writePrisms(fileName, numberOfPrisms, prisms, doByteSwap);
    UgridWriter::writeHexs(fileName, numberOfHexes, hexes, doByteSwap);
}
