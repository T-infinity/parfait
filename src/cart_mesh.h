#ifndef CART_MESH_H
#define CART_MESH_H
#include "point.h"

#include "cart_block.h"
#include <vector>

namespace Parfait {
    class CartMesh {
    public:
        CartMesh(
                const Point<double> &lo,
                const Point<double> &hi,
                int numCellsX,
                int numCellsY,
                int numCellsZ
        );

        int numberOfCells() const { return block.numberOfCells(); }

        int numberOfNodes() const;
        int numberOfNodesInCell(int Id) const;
        int numberOfNodesInCellFace(int cellId, int faceId) const;
        int numberOfFacesInCell(int cellId) const;

        std::vector<int> getNodesInCellFace(int cellId, int faceId) const;
        std::vector<int> getNodesInCell(int cellId) const;

        int numberOfBoundaryFaces() const;
        int faceNeighbor(int cellId, int faceId) const;
        int faceBoundaryTag(int cellId, int faceId) const;
        bool isFaceBoundary(int cellId, int faceId) const;
        Point<double> faceArea(int cellId, int faceId) const;

        Point<double> cellCentroid(int cellId) const;
        double cellVolume(int cellId) const;

        void getNode(int nodeId, double node[3]) const;
        void getNode(int i, int j, int k, double node[3]) const;
        Point<double> getNode(int nodeId) const;
        int getNodeId(int i, int j, int k) const;
        void getLogicalDimensions(int &nX, int &nY, int &nZ) const;
        void getSpacings(double &hx, double &hy, double &hz) const;
        void getLogicalCoordinates(int nodeId, int &i, int &j, int &k) const;

    private:
        CartBlock block;
        double volume;

        int numberOfCellsOnSide(int faceId) const;
        int whatSideIsCellFaceOn(int cellId, int faceId) const;
    };


}

#include "cart_mesh.hpp"

#endif
