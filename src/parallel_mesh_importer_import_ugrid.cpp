#include "parallel_mesh_importer.h"

ImportedUgrid ParallelMeshImporter::importUgrid()
{
	ImportedUgrid ugrid(myNodes,myTriangles,myQuads,myTets,
			myPyramids,myPrisms,myHexs,myTriangleTags,myQuadTags);
	return ugrid;
}
