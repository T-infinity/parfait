#include "ugrid_reader.h"
#include "byteswap.h"
#include <stdexcept>


void UgridReader::readHeader(std::string &filename,int &nnodes,
		int &ntri,int &nquad,
		int &ntet,int &npyr,int &nprism,int &nhex)
{
	readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,false);
}

std::vector<double> UgridReader::readNodes(std::string &filename)
{
	return readNodes(filename,false);
}

std::vector<int> UgridReader::readTriangles(std::string &filename)
{
	return readTriangles(filename,false);
}

std::vector<int> UgridReader::readQuads(std::string &filename)
{
	return readQuads(filename,false);
}

std::vector<int> UgridReader::readTets(std::string &filename)
{
	return readTets(filename,false);
}

std::vector<int> UgridReader::readPyramids(std::string &filename)
{
	return readPyramids(filename,false);
}

std::vector<int> UgridReader::readPrisms(std::string &filename)
{
	return readPrisms(filename,false);
}

std::vector<int> UgridReader::readHexs(std::string &filename)
{
	return readHexs(filename,false);
}

std::vector<int> UgridReader::readTriangleBoundaryTags(std::string &filename)
{
	return readTriangleBoundaryTags(filename,false);
}

std::vector<int> UgridReader::readQuadBoundaryTags(std::string &filename)
{
	return readQuadBoundaryTags(filename,false);
}

std::vector<double> UgridReader::readNodes(std::string &filename,
		int begin,int end)
{
	return readNodes(filename,begin,end,false);
}

std::vector<int> UgridReader::readTriangles(std::string &filename,
		int begin,int end)
{
	return readTriangles(filename,begin,end,false);
}

std::vector<int> UgridReader::readQuads(std::string &filename,
		int begin,int end)
{
	return readQuads(filename,begin,end,false);
}

std::vector<int> UgridReader::readTets(std::string &filename,
		int begin,int end)
{
	return readTets(filename,begin,end,false);
}

std::vector<int> UgridReader::readPyramids(std::string &filename,
		int begin,int end)
{
	return readPyramids(filename,begin,end,false);
}

std::vector<int> UgridReader::readPrisms(std::string &filename,
		int begin,int end)
{
	return readPrisms(filename,begin,end,false);
}

std::vector<int> UgridReader::readHexs(std::string &filename,
		int begin,int end)
{
	return readHexs(filename,begin,end,false);
}

std::vector<int> UgridReader::readBoundaryTags(std::string &filename,
		int begin,int end)
{
	return readBoundaryTags(filename,begin,end,false);
}

std::vector<int> UgridReader::readTriangleBoundaryTags(std::string &filename,
		int begin,int end)
{
	return readTriangleBoundaryTags(filename,begin,end,false);
}

std::vector<int> UgridReader::readQuadBoundaryTags(std::string &filename,
		int begin,int end)
{
	return readQuadBoundaryTags(filename,begin,end,false);
}











void UgridReader::readHeader(std::string &filename,int &nnodes,
        int &ntri,int &nquad,
        int &ntet,int &npyr,int &nprism,int &nhex,bool swapBytes)
{
    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
	if(swapBytes)
	{
		private_fread(&nnodes,sizeof(int),1,f);
		private_fread(&ntri  ,sizeof(int),1,f);
		private_fread(&nquad ,sizeof(int),1,f);
		private_fread(&ntet  ,sizeof(int),1,f);
		private_fread(&npyr  ,sizeof(int),1,f);
		private_fread(&nprism,sizeof(int),1,f);
		private_fread(&nhex  ,sizeof(int),1,f);
	}
	else
	{
		fread(&nnodes,sizeof(int),1,f);
		fread(&ntri  ,sizeof(int),1,f);
		fread(&nquad ,sizeof(int),1,f);
		fread(&ntet  ,sizeof(int),1,f);
		fread(&npyr  ,sizeof(int),1,f);
		fread(&nprism,sizeof(int),1,f);
		fread(&nhex  ,sizeof(int),1,f);
	}
    fclose(f);
}

std::vector<double> UgridReader::readNodes(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readNodes(filename,0,nnodes,swapBytes);
}

std::vector<double> UgridReader::readNodes(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<double> nodes(3*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
	int byteOffset = 7*sizeof(int);
	byteOffset += 3*begin*sizeof(double);
	fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
		private_fread(&nodes[0],sizeof(double),3*nrequested,f);
	else
		fread(&nodes[0],sizeof(double),3*nrequested,f);
	fclose(f);
    return nodes;
}

std::vector<int> UgridReader::readTriangles(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readTriangles(filename,0,ntri,swapBytes);
}

std::vector<int> UgridReader::readTriangles(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> triangles(3*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&triangles[0],sizeof(int),3*nrequested,f);
	else
    	fread(&triangles[0],sizeof(int),3*nrequested,f);
    fclose(f);

    for(int& vertex : triangles)
        vertex--;
    return triangles;
}

std::vector<int> UgridReader::readQuads(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readQuads(filename,0,nquad,swapBytes);
}

std::vector<int> UgridReader::readQuads(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> quads(4*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&quads[0],sizeof(int),4*nrequested,f);
	else
    	fread(&quads[0],sizeof(int),4*nrequested,f);
    fclose(f);

    for(int& vertex : quads)
        vertex--;

    return quads;
}

std::vector<int> UgridReader::readTets(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readTets(filename,0,ntet,swapBytes);
}

std::vector<int> UgridReader::readTets(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> tets(4*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&tets[0],sizeof(int),4*nrequested,f);
	else
    	fread(&tets[0],sizeof(int),4*nrequested,f);
    fclose(f);

    for(int& vertex : tets)
        vertex--;

    return tets;
}

std::vector<int> UgridReader::readPyramids(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readPyramids(filename,0,npyr,swapBytes);
}

std::vector<int> UgridReader::readPyramids(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> pyrs(5*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*ntet*sizeof(int);
    byteOffset += 4*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&pyrs[0],sizeof(int),5*nrequested,f);
	else
    	fread(&pyrs[0],sizeof(int),5*nrequested,f);
    fclose(f);

    for(int& vertex : pyrs)
        vertex--;

    return pyrs;
}

std::vector<int> UgridReader::readPrisms(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readPrisms(filename,0,nprism,swapBytes);
}

std::vector<int> UgridReader::readPrisms(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> prisms(6*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*ntet*sizeof(int);
    byteOffset += 5*npyr*sizeof(int);
    byteOffset += 6*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&prisms[0],sizeof(int),6*nrequested,f);
	else
    	fread(&prisms[0],sizeof(int),6*nrequested,f);
    fclose(f);

    for(int& vertex : prisms)
        vertex--;

    return prisms;
}

std::vector<int> UgridReader::readHexs(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readHexs(filename,0,nhex,swapBytes);
}

std::vector<int> UgridReader::readHexs(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> hexs(8*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*ntet*sizeof(int);
    byteOffset += 5*npyr*sizeof(int);
    byteOffset += 6*nprism*sizeof(int);
    byteOffset += 8*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&hexs[0],sizeof(int),8*nrequested,f);
	else
    	fread(&hexs[0],sizeof(int),8*nrequested,f);
    fclose(f);

    for(int& vertex : hexs)
        vertex--;

    return hexs;
}

std::vector<int> UgridReader::readTriangleBoundaryTags(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,0,ntri,swapBytes);
}

std::vector<int> UgridReader::readTriangleBoundaryTags(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,begin,end,swapBytes);
}

std::vector<int> UgridReader::readQuadBoundaryTags(std::string &filename,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,ntri,ntri+nquad,swapBytes);
}

std::vector<int> UgridReader::readQuadBoundaryTags(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,ntri+begin,ntri+end,swapBytes);
}

std::vector<int> UgridReader::readBoundaryTags(std::string &filename,int begin,int end,bool swapBytes)
{
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> tags(nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
	if(swapBytes)
    	private_fread(&tags[0],sizeof(int),nrequested,f);
    else
		fread(&tags[0],sizeof(int),nrequested,f);
    fclose(f);
    return tags;
}
