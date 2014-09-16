#ifndef UGRID_WRITER_H
#define UGRID_WRITER_H
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include "imported_ugrid.h"
namespace UgridWriter
{

    void writeImportedUgrid(ImportedUgrid &ugrid, std::string filename);

	/// Tools for writing files in native endianness
	void writeHeader(std::string &filename,int nnodes,
		int ntri,int nquad,
		int ntet,int npyr,int prism,int hex,bool swapBytes);

	void writeNodes(std::string &filename,int nnodes,double *nodes,bool swapBytes);
	void writeTriangles(   std::string &filename,int ntriangles,int *triangles,bool swapBytes);
	void writeQuads(       std::string &filename,int nquads,    int *quads,    bool swapBytes);
	void writeTets(        std::string &filename,int ntets     ,int *tets     ,bool swapBytes);
	void writePyramids(    std::string &filename,int npyramyds ,int *pyramids ,bool swapBytes);
	void writePrisms(      std::string &filename,int nprisms   ,int *prisms   ,bool swapBytes);
	void writeHexs(        std::string &filename,int nhexs     ,int *hexs     ,bool swapBytes);
	void writeTriangleBoundaryTags(std::string &filename,int ntriangles,int *triangleTags,bool swapBytes);
	void writeQuadBoundaryTags(    std::string &filename,int nquads,    int *quadTags,bool swapBytes);

	void writeIntegerField(std::string &filename,int n,int *fieldData,bool swapBytes);
}

#endif
