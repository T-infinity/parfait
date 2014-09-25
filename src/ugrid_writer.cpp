#include "ugrid_writer.h"
#include "byteswap.h"
#include "message_passer.h"
#include <stdexcept>
#include <vector>

using std::vector;

void UgridWriter::writeImportedUgrid(ImportedUgrid &ugrid, std::string filename, bool swapBytes)
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
    	writeHeader(filename, ugrid.nodes.size() / 3, ugrid.triangles.size() / 3, 
            ugrid.quads.size() / 4, ugrid.tets.size() / 4, 
            ugrid.pyramids.size() / 5, ugrid.prisms.size() / 6, ugrid.hexs.size() / 8, swapBytes);
	}

	// first make lists of nodes and elements
	vector<double> myNodes(ugrid.numberOfNodes(),0.0);
	vector<int>    myTets;
	vector<int>    myPyrs;
	vector<int>    myPrisms;
	vector<int>    myHexs;


	// then transfer to the root process and write them out
	for(int i=0;i<NumberOfProcesses();i++)
	{
		
	}
    writeNodes(filename, ugrid.nodes.size() / 3, ugrid.nodes.data(), swapBytes);
    writeTriangles(filename, ugrid.triangles.size() / 3, ugrid.triangles.data(), swapBytes);
    writeQuads(filename, ugrid.quads.size() / 4, ugrid.quads.data(), swapBytes);
    writeTriangleBoundaryTags(filename, ugrid.triangles.size() / 3, ugrid.triangleTags.data(), swapBytes);
    writeQuadBoundaryTags(filename, ugrid.quads.size() / 4, ugrid.quadTags.data(), swapBytes);

    writeTets(filename, ugrid.tets.size() / 4, ugrid.tets.data(), swapBytes);
    writePyramids(filename, ugrid.pyramids.size() / 5, ugrid.pyramids.data(), swapBytes);
    writePrisms(filename, ugrid.prisms.size() / 6, ugrid.prisms.data(), swapBytes);
    writeHexs(filename, ugrid.hexs.size() / 8, ugrid.hexs.data(), swapBytes);
}

void UgridWriter::writeHeader(std::string &filename,int nnodes,
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

void UgridWriter::writeNodes(std::string &filename,int nnodes,double *nodes,bool swapBytes)
{
	FILE *f = fopen(filename.c_str(),"ab");
	assert(f != NULL);
	if(swapBytes)
		private_fwrite(nodes,sizeof(double),3*nnodes,f);
	else
		fwrite(nodes,sizeof(double),3*nnodes,f);
	fclose(f);
}

void UgridWriter::writeTriangles(std::string &filename,int ntriangles,int *triangles,bool swapBytes)
{
	vector<int> tris(3*ntriangles,0);
	for(int i=0;i<3*ntriangles;i++) tris[i] = triangles[i]+1;
	writeIntegerField(filename,3*ntriangles,tris.data(),swapBytes);
}

void UgridWriter::writeQuads(std::string &filename,int nquads,int *quads,bool swapBytes)
{
	vector<int> newQuads(4*nquads,0);
	for(int i=0;i<4*nquads;i++) newQuads[i] = quads[i]+1;
	writeIntegerField(filename,4*nquads,newQuads.data(),swapBytes);
}

void UgridWriter::writeTets(std::string &filename,int ntets,int *tets,bool swapBytes)
{
	vector<int> newTets(4*ntets,0);
	for(int i=0;i<4*ntets;i++) newTets[i] = tets[i]+1;
	writeIntegerField(filename,4*ntets,newTets.data(),swapBytes);
}

void UgridWriter::writePyramids(std::string &filename,int npyramids,int *pyramids,bool swapBytes)
{
	vector<int> newPyramids(5*npyramids,0);
	for(int i=0;i<5*npyramids;i++) newPyramids[i] = pyramids[i]+1;
	writeIntegerField(filename,5*npyramids,newPyramids.data(),swapBytes);
}

void UgridWriter::writePrisms(std::string &filename,int nprisms,int *prisms,bool swapBytes)
{
	vector<int> newPrisms(6*nprisms,0);
	for(int i=0;i<6*nprisms;i++) newPrisms[i] = prisms[i]+1;
	writeIntegerField(filename,6*nprisms,newPrisms.data(),swapBytes);
}

void UgridWriter::writeHexs(std::string &filename,int nhexs,int *hexs,bool swapBytes)
{
	vector<int> newHexs(8*nhexs,0);
	for(int i=0;i<8*nhexs;i++) newHexs[i] = hexs[i]+1;
	writeIntegerField(filename,8*nhexs,newHexs.data(),swapBytes);
}

void UgridWriter::writeTriangleBoundaryTags(std::string &filename,int ntriangles,int *triangleTags,bool swapBytes)
{
	writeIntegerField(filename,ntriangles,triangleTags,swapBytes);
}

void UgridWriter::writeQuadBoundaryTags(std::string &filename,int nquads,int *quadTags,bool swapBytes)
{
	writeIntegerField(filename,nquads,quadTags,swapBytes);
}


void UgridWriter::writeIntegerField(std::string &filename,int n,int *fieldData,bool swapBytes)
{
	FILE *f = fopen(filename.c_str(),"ab");
	assert(f != NULL);
	if(swapBytes)
		private_fwrite(fieldData,sizeof(int),n,f);
	else
		fwrite(fieldData,sizeof(int),n,f);
	fclose(f);
}
