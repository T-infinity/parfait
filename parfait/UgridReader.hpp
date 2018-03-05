#include "ByteSwap.h"
#include <stdexcept>

inline void Parfait::UgridReader::readHeader(std::string filename,int &nnodes,
		int &ntri,int &nquad,
		int &ntet,int &npyr,int &nprism,int &nhex) {
	readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,false);
}

inline std::vector<double> Parfait::UgridReader::readNodes(std::string filename) {
	return readNodes(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readTriangles(std::string filename) {
	return readTriangles(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readQuads(std::string filename) {
	return readQuads(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readTets(std::string filename) {
	return readTets(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readPyramids(std::string filename) {
	return readPyramids(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readPrisms(std::string filename) {
	return readPrisms(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readHexs(std::string filename) {
	return readHexs(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readTriangleBoundaryTags(std::string filename) {
	return readTriangleBoundaryTags(filename,false);
}

inline std::vector<int> Parfait::UgridReader::readQuadBoundaryTags(std::string filename) {
	return readQuadBoundaryTags(filename,false);
}

inline std::vector<double> Parfait::UgridReader::readNodes(std::string filename, int begin,int end) {
	return readNodes(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readTriangles(std::string filename, int begin,int end) {
	return readTriangles(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readQuads(std::string filename, int begin,int end) {
	return readQuads(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readTets(std::string filename, int begin,int end) {
	return readTets(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readPyramids(std::string filename, int begin,int end) {
	return readPyramids(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readPrisms(std::string filename, int begin,int end) {
	return readPrisms(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readHexs(std::string filename, int begin,int end) {
	return readHexs(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readBoundaryTags(std::string filename, int begin,int end) {
	return readBoundaryTags(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readTriangleBoundaryTags(std::string filename, int begin,int end) {
	return readTriangleBoundaryTags(filename,begin,end,false);
}

inline std::vector<int> Parfait::UgridReader::readQuadBoundaryTags(std::string filename, int begin,int end) {
	return readQuadBoundaryTags(filename,begin,end,false);
}

inline void Parfait::UgridReader::readHeader(std::string filename,int &nnodes, int &ntri,int &nquad, int &ntet,int &npyr,int &nprism,int &nhex,bool swapBytes) {
    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    fread(&nnodes,sizeof(int),1,f);
    fread(&ntri  ,sizeof(int),1,f);
    fread(&nquad ,sizeof(int),1,f);
    fread(&ntet  ,sizeof(int),1,f);
    fread(&npyr  ,sizeof(int),1,f);
    fread(&nprism,sizeof(int),1,f);
    fread(&nhex  ,sizeof(int),1,f);
	if(swapBytes) {
		bswap_32(&nnodes);
		bswap_32(&ntri  );
		bswap_32(&nquad );
		bswap_32(&ntet  );
		bswap_32(&npyr  );
		bswap_32(&nprism);
		bswap_32(&nhex  );
	}
    fclose(f);
}

inline std::vector<double> Parfait::UgridReader::readNodes(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readNodes(filename,0,nnodes,swapBytes);
}

inline std::vector<double> Parfait::UgridReader::readNodes(std::string filename,int begin,int end,bool swapBytes) {
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
		fread(&nodes[0],sizeof(double),3*nrequested,f);
    if(swapBytes) {
        for (size_t i = 0; i < 3 * nrequested; i++) {
            bswap_64(&nodes[i]);
        }
    }

	fclose(f);
    return nodes;
}

inline std::vector<int> Parfait::UgridReader::readTriangles(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readTriangles(filename,0,ntri,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readTriangles(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> triangles(3*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&triangles[0],sizeof(int),3*nrequested,f);
    fclose(f);
    if(swapBytes) {
        for (size_t i = 0; i < 3 * nrequested; i++) {
            bswap_32(&triangles[i]);
        }
    }

    for(int& vertex : triangles)
        vertex--;
    return triangles;
}

inline std::vector<int> Parfait::UgridReader::readQuads(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    return readQuads(filename,0,nquad,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readQuads(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> quads(4*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&quads[0],sizeof(int),4*nrequested,f);
    fclose(f);
    if(swapBytes) {
        for (size_t i = 0; i < 4 * nrequested; i++) {
            bswap_32(&quads[i]);
        }
    }

    for(int& vertex : quads)
        vertex--;

    return quads;
}

inline std::vector<int> Parfait::UgridReader::readTets(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readTets(filename,0,ntet,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readTets(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> tets(4*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&tets[0],sizeof(int),4*nrequested,f);
    fclose(f);

    if(swapBytes) {
        for (size_t i = 0; i < 4 * nrequested; i++) {
            bswap_32(&tets[i]);
        }
    }

    for(int& vertex : tets)
        vertex--;

    return tets;
}

inline std::vector<int> Parfait::UgridReader::readPyramids(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readPyramids(filename,0,npyr,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readPyramids(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> pyrs(5*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*ntet*sizeof(int);
    byteOffset += 5*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&pyrs[0],sizeof(int),5*nrequested,f);
    fclose(f);
    if(swapBytes) {
        for (size_t i = 0; i < 5 * nrequested; i++) {
            bswap_32(&pyrs[i]);
        }
    }

    for(int& vertex : pyrs)
        vertex--;

    return pyrs;
}

inline std::vector<int> Parfait::UgridReader::readPrisms(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readPrisms(filename,0,nprism,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readPrisms(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> prisms(6*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*ntet*sizeof(int);
    byteOffset += 5*npyr*sizeof(int);
    byteOffset += 6*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&prisms[0],sizeof(int),6*nrequested,f);
    if(swapBytes) {
        for (size_t i = 0; i < 6 * nrequested; i++) {
            bswap_32(&prisms[i]);
        }
    }
    fclose(f);

    for(int& vertex : prisms) // decrement to C indexing
        vertex--;
    for(auto id:prisms) {
        if (id < 0) {
            printf("begin %i end %i",begin,end);
            throw std::logic_error("read negative from disk");
        }
    }
    return prisms;
}

inline std::vector<int> Parfait::UgridReader::readHexs(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readHexs(filename,0,nhex,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readHexs(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> hexs(8*nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += (ntri+nquad)*sizeof(int);
    byteOffset += 4*ntet*sizeof(int);
    byteOffset += 5*npyr*sizeof(int);
    byteOffset += 6*nprism*sizeof(int);
    byteOffset += 8*begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&hexs[0],sizeof(int),8*nrequested,f);
    fclose(f);
    if(swapBytes) {
        for (size_t i = 0; i < 8 * nrequested; i++) {
            bswap_32(&hexs[i]);
        }
    }

    for(int& vertex : hexs)
        vertex--;

    return hexs;
}

inline std::vector<int> Parfait::UgridReader::readTriangleBoundaryTags(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,0,ntri,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readTriangleBoundaryTags(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,begin,end,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readQuadBoundaryTags(std::string filename,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,ntri,ntri+nquad,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readQuadBoundaryTags(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);	
    return readBoundaryTags(filename,ntri+begin,ntri+end,swapBytes);
}

inline std::vector<int> Parfait::UgridReader::readBoundaryTags(std::string filename,int begin,int end,bool swapBytes) {
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    // get header info and allocate space for triangles
    readHeader(filename,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,swapBytes);
    int nrequested = end - begin;
    std::vector<int> tags(nrequested,0);

    FILE *f = fopen(filename.c_str(),"rb");
    if(f == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    unsigned long int byteOffset = 7*sizeof(int) + 3*nnodes*sizeof(double);
    byteOffset += 3*ntri*sizeof(int);
    byteOffset += 4*nquad*sizeof(int);
    byteOffset += begin*sizeof(int);
    fseek(f,byteOffset,SEEK_SET);
    fread(&tags[0],sizeof(int),nrequested,f);
    fclose(f);
    if(swapBytes) {
        for (size_t i = 0; i < nrequested; i++) {
            bswap_32(&tags[i]);
        }
    }
    return tags;
}
