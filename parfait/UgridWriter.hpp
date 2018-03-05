#include "ByteSwap.h"
#include <stdexcept>
#include <vector>

using namespace Parfait;

inline void UgridWriter::writeImportedUgrid(ImportedUgrid &ugrid, std::string filename, bool swapBytes) {
    int numberOfNodes      = ugrid.numberOfNodes();
    int numberOfTets       = ugrid.tets.size()/4;
    int numberOfPyramids   = ugrid.pyramids.size()/5;
    int numberOfPrisms     = ugrid.prisms.size()/6;
    int numberOfHexs       = ugrid.hexs.size()/8;
    int numberOfTriangles  = ugrid.triangles.size()/3;
    int numberOfQuads      = ugrid.quads.size()/4;

    writeHeader(filename, numberOfNodes, numberOfTriangles, 
            numberOfQuads, numberOfTets, numberOfPyramids, 
            numberOfPrisms, numberOfHexs, swapBytes);

    writeNodes(filename, ugrid.nodes.size()/3, ugrid.nodes.data(), swapBytes);
    writeTriangles(filename, ugrid.triangles.size() / 3, ugrid.triangles.data(), swapBytes);
    writeQuads(filename, ugrid.quads.size() / 4, ugrid.quads.data(), swapBytes);
    writeTriangleBoundaryTags(filename, ugrid.triangles.size() / 3, 
            ugrid.triangleTags.data(), swapBytes);
    writeQuadBoundaryTags(filename, ugrid.quads.size() / 4, ugrid.quadTags.data(), swapBytes);
    writeTets(filename, ugrid.tets.size() / 4, ugrid.tets.data(), swapBytes);
    writePyramids(filename, ugrid.pyramids.size() / 5, ugrid.pyramids.data(), swapBytes);
    writePrisms(filename, ugrid.prisms.size() / 6, ugrid.prisms.data(), swapBytes);
    writeHexs(filename, ugrid.hexs.size() / 8, ugrid.hexs.data(), swapBytes);
}

inline void UgridWriter::writeHeader(std::string filename,int nnodes,
        int ntri,int nquad,
        int ntet,int npyr,int nprism,int nhex,bool swapBytes) {
    FILE *f = fopen(filename.c_str(),"wb");
    if(f == NULL)
        throw std::domain_error("Could not open .ugrid file: "+filename);
    if(swapBytes){
        bswap_32(&nnodes);
        bswap_32(&ntri);
        bswap_32(&nquad);
        bswap_32(&ntet);
        bswap_32(&npyr);
        bswap_32(&nprism);
        bswap_32(&nhex);
    }

    fwrite(&nnodes,sizeof(int),1,f);
    fwrite(&ntri  ,sizeof(int),1,f);
    fwrite(&nquad ,sizeof(int),1,f);
    fwrite(&ntet  ,sizeof(int),1,f);
    fwrite(&npyr  ,sizeof(int),1,f);
    fwrite(&nprism,sizeof(int),1,f);
    fwrite(&nhex  ,sizeof(int),1,f);
    fclose(f);
}

inline void UgridWriter::writeNodes(std::string filename,int nnodes,double *nodes,bool swapBytes) {
	FILE *f = fopen(filename.c_str(),"ab");
    if(f == NULL)
        throw std::domain_error("Could not open .ugrid file: "+filename);
    for(size_t i = 0; i < 3*nnodes; i++){
        bswap_64(&nodes[i]);
    }
    fwrite(nodes,sizeof(double),3*nnodes,f);
    for(size_t i = 0; i < 3*nnodes; i++){
        bswap_64(&nodes[i]);
    }
	fclose(f);
}

inline void UgridWriter::writeTriangles(std::string filename,int ntriangles,int *triangles,bool swapBytes) {
	std::vector<int> tris(3*ntriangles,0);
	for(int i=0;i<3*ntriangles;i++) tris[i] = triangles[i]+1;
	writeIntegerField(filename,3*ntriangles,tris.data(),swapBytes);
}

inline void UgridWriter::writeQuads(std::string filename,int nquads,int *quads,bool swapBytes) {
	std::vector<int> newQuads(4*nquads,0);
	for(int i=0;i<4*nquads;i++) newQuads[i] = quads[i]+1;
	writeIntegerField(filename,4*nquads,newQuads.data(),swapBytes);
}

inline void UgridWriter::writeTets(std::string filename,int ntets,int *tets,bool swapBytes) {
	std::vector<int> newTets(4*ntets,0);
	for(int i=0;i<4*ntets;i++) newTets[i] = tets[i]+1;
	writeIntegerField(filename,4*ntets,newTets.data(),swapBytes);
}

inline void UgridWriter::writePyramids(std::string filename,int npyramids,int *pyramids,bool swapBytes) {
	std::vector<int> newPyramids(5*npyramids,0);
	for(int i=0;i<5*npyramids;i++) newPyramids[i] = pyramids[i]+1;
	writeIntegerField(filename,5*npyramids,newPyramids.data(),swapBytes);
}

inline void UgridWriter::writePrisms(std::string filename,int nprisms,int *prisms,bool swapBytes) {
	std::vector<int> newPrisms(6*nprisms,0);
	for(int i=0;i<6*nprisms;i++) newPrisms[i] = prisms[i]+1;
	writeIntegerField(filename,6*nprisms,newPrisms.data(),swapBytes);
}

inline void UgridWriter::writeHexs(std::string filename,int nhexs,int *hexs,bool swapBytes) {
	std::vector<int> newHexs(8*nhexs,0);
	for(int i=0;i<8*nhexs;i++) newHexs[i] = hexs[i]+1;
	writeIntegerField(filename,8*nhexs,newHexs.data(),swapBytes);
}

inline void UgridWriter::writeTriangleBoundaryTags(std::string filename,int ntriangles,int *triangleTags,bool swapBytes) {
	writeIntegerField(filename,ntriangles,triangleTags,swapBytes);
}

inline void UgridWriter::writeQuadBoundaryTags(std::string filename,int nquads,int *quadTags,bool swapBytes) {
	writeIntegerField(filename,nquads,quadTags,swapBytes);
}


inline void UgridWriter::writeIntegerField(std::string filename,int n,int *fieldData,bool swapBytes) {
	FILE *f = fopen(filename.c_str(),"ab");
	assert(f != NULL);
	if(swapBytes){
        for(size_t i = 0; i < n; i++){
            bswap_32(&fieldData[i]);
        }
    }

    fwrite(fieldData,sizeof(int),n,f);
	fclose(f);
    if(swapBytes){
        for(size_t i = 0; i < n; i++){
            bswap_32(&fieldData[i]);
        }
    }
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
