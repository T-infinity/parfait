#ifndef FUN3D_MESH_H
#define FUN3D_MESH_H
#include <assert.h>
#include <stdlib.h>
#include "generic_mesh.h"
#include <vector>

//-------------------------------------------------------------
// This serves as an example of a mesh format that meets the
// criteria of a generic mesh that can be used with any of
// the templated tools.
//
// It specifically provides an interface for native Fun3D data
// ------------------------------------------------------------

using std::vector;
namespace Parfait { ;

    class Fun3DMesh {
    public:
        // Any Generic mesh must implement the following
        // functions (no inheritance required)
        void getNode(int nodeId, double node[3]) const;
        int numberOfNodes() const;
        int numberOfCells() const;
        int numberOfNodesInCell(int id) const;
        int numberOfFacesInCell(int id) const;
        int numberOfNodesInCellFace(int cellId, int faceId) const;
        vector<int> getNodesInCellFace(int cellId, int faceId) const;
        vector<int> getVtkOrderedNodesInCell(int cellId) const;
        void getNodesInCell(int cellId, int cell[]) const;
        vector<int> getNodesInCell(int cellId) const;

        // Any Generic Surface mesh must implement the following
        int numberOfBoundaryFaces() const;
        int numberOfNodesInBoundaryFace(int boundaryFaceId) const;
        vector<int> getNodesInBoundaryFace(int boundaryFaceId) const;
        void getNodesInBoundaryFace(int boundaryFaceId, int *face) const;


        // everything below this point is specific to Fun3D
        int getGlobalNodeId(int localNodeId);
        int getImesh(int localNodeId);
        int getBoundaryTag(int boundaryFaceId);
        int getBoundaryCondition(int boundaryFaceId);

        int numberOfNonGhostNodes() { return nnodes0; }

        void setNode(int nodeId, double node[3]);
        Fun3DMesh(int nnodes0, int nnodes01, double *x, double *y, double *z,
                  int *globalNodeIds,
                  int *imesh,
                  int ntriangles, int *triangles, int *triangleTags, int *triangleBcs,
                  int nquads, int *quads, int *quadTags, int *quadBcs,
                  int ntets, int *tets,
                  int npyramids, int *pyramids,
                  int nprisms, int *prisms,
                  int nhexs, int *hexs);
        void freePointers();

        int *imesh;
    private:
        int nnodes0, nnodes01, ntriangles, nquads;
        int ntets, npyramids, nprisms, nhexs;
        int ncells, nfaces;
        double *x, *y, *z;
        int *globalNodeIds;
        int *triangles, *quads, *triangleTags, *quadTags, *triangleBcs, *quadBcs;
        int *tets, *pyramids, *prisms, *hexs;
        int cellMap[4];
        int faceMap[2];
    };
}
#endif