#ifndef IMPORTED_UGRID_H
#define IMPORTED_UGRID_H
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include "MapbcReader.h"
#include "Point.h"

//-------------------------------------------------------------
// This serves as an example of a mesh format that meets the
// criteria of a generic mesh that can be used with any of
// the templated tools.
// ------------------------------------------------------------
namespace Parfait {
    class ImportedUgrid {
    public:
        // Any Generic mesh must implement the following
        // functions (no inheritance required)
        void getNode(int nodeId, double node[3]) const;
        Point<double> getNode(int nodeId) const;
        int numberOfNodes() const;
        int numberOfCells() const;
        int numberOfNodesInCell(int id) const;
        int numberOfFacesInCell(int id) const;
        int numberOfNodesInCellFace(int cellId, int faceId) const;

        std::vector<int> getNodesInCellFace(int cellId, int faceId) const;

        std::vector<int> getNodesInCell(int cellId) const;
        void getNodesInCell(int cellId, int *cell) const;
        std::vector<int> getVtkOrderedNodesInCell(int cellId) const;

        void setNode(int nodeId, double node[3]);

        // Any Generic Surface mesh must implement the following
        int numberOfBoundaryFaces() const;
        int numberOfNodesInBoundaryFace(int boundaryFaceId) const;

        std::vector<int> getNodesInBoundaryFace(int boundaryFaceId) const;
        void getNodesInBoundaryFace(int boundaryFaceId, int *face) const;

        // Everything below is specific to ImportedUgrid and
        // has nothing to do with any other generic mesh
        // (i.e., this stuff doesn't have to be in your mesh)

        int getBoundaryTag(int boundaryFaceId) const;
        int getBoundaryCondition(int BoundaryFaceId) const;

        enum {TET = 0, PYRAMID = 1, PRISM = 2, HEX = 3};
        int getCellType(int cellId) const;

        ImportedUgrid(std::vector<double> nodes, std::vector<int> triangles,
                      std::vector<int> quads, std::vector<int> tets, std::vector<int> pyramids,
                      std::vector<int> prisms, std::vector<int> hexs,
                      std::vector<int> triangleTags, std::vector<int> quadTags,
                      std::vector<int> triangleBoundaryConditions,
                      std::vector<int> quadBoundaryConditions);
        std::vector<double> nodes;
        std::vector<int> cellMap;
        std::vector<int> faceMap;
        std::vector<int> triangles;
        std::vector<int> quads;
        std::vector<int> tets;
        std::vector<int> pyramids;
        std::vector<int> prisms;
        std::vector<int> hexs;
        std::vector<int> triangleTags;
        std::vector<int> quadTags;
        std::vector<int> triangleBoundaryConditions;
        std::vector<int> quadBoundaryConditions;
    protected:
        int nnodes;
        int ncells;
        int nfaces;
    };

}

#include "ImportedUgrid.hpp"

#endif
