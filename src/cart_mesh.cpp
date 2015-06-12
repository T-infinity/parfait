#include <stdexcept>
#include "cart_mesh.h"
#include "range_loop.h"
#include <string>

using namespace Parfait;
CartMesh::CartMesh(
        const Point<double> &lo,
        const Point<double> &hi,
        int numCellsX,
        int numCellsY,
        int numCellsZ )
    : block(&lo[0], &hi[0], numCellsX, numCellsY, numCellsZ){

        volume = block.get_dx() * block.get_dy() * block.get_dz();
    }

int CartMesh::numberOfBoundaryFaces() const
{
    int nx = block.numberOfCells_X();
    int ny = block.numberOfCells_Y();
    int nz = block.numberOfCells_Z();
    return 2*(nx*ny) + 2*(nx*nz) + 2*(ny*nz); 
}

void CartMesh::getNode(int nodeId, double node[3]) const 
{
    block.getNode(nodeId, node);
}

void CartMesh::getNode(int i,int j,int k,double node[3]) const
{
    int nodeId = block.convert_ijk_ToNodeId(i,j,k);
	block.getNode(nodeId,node);
}

int CartMesh::getNodeId(int i,int j,int k) const
{
	return block.convert_ijk_ToNodeId(i,j,k);
}

void CartMesh::getLogicalDimensions(int &nx,int &ny,int &nz) const
{
	nx = block.numberOfNodes_X();
	ny = block.numberOfNodes_Y();
	nz = block.numberOfNodes_Z();
}

void CartMesh::getSpacings(double &hx,double &hy,double &hz) const
{
	hx = block.get_dx();
	hy = block.get_dy();
	hz = block.get_dz();
}

void CartMesh::getLogicalCoordinates(int nodeId,int &i,int &j,int &k) const
{
	block.convertNodeIdTo_ijk(nodeId,i,j,k);
}

int CartMesh::numberOfNodes() const
{
    return block.numberOfNodes();
}

int CartMesh::numberOfNodesInCell(int cellId) const 
{
    return 8;
}

int CartMesh::numberOfNodesInCellFace(int cellId, int faceId) const
{
    return 4;
}

int CartMesh::numberOfFacesInCell(int cellId) const
{
    return 6;
}

int CartMesh::faceNeighbor(int cellId, int faceId) const 
{
    int i, j, k;
    block.convertCellIdTo_ijk(cellId, i, j, k);

    int neighbor = numberOfCells();
    switch (faceId) {
        case 0:
            k--;
            if(k < 0)
            {
                neighbor += j*block.numberOfCells_X() + i;
                return neighbor;
            }
            break;
        case 1:
            j--;
            if(j < 0)
            {
                neighbor += numberOfCellsOnSide(0);
                neighbor += k*block.numberOfCells_X() + i;
                return neighbor;
            }
            break;
        case 2:
            i++;
            if(i >= block.numberOfCells_X())
            {
                neighbor += numberOfCellsOnSide(0);
                neighbor += numberOfCellsOnSide(1);
                neighbor += j + k*block.numberOfCells_Y();
                return neighbor;
            }
            break;
        case 3:
            j++;
            if(j >= block.numberOfCells_Y())
            {
                for(int side : range(3)){
                    neighbor += numberOfCellsOnSide(side);
                }
                neighbor += i + k*block.numberOfCells_X();
                return neighbor;
            }
            break;
        case 4:
            i--;
            if(i < 0)
            {
                for(int side : range(4)){
                    neighbor += numberOfCellsOnSide(side);
                }
                neighbor += j + k*block.numberOfCells_Y();
                return neighbor;
            }
            break;
        case 5:
            k++;
            if(k >= block.numberOfCells_Z())
            {
                for(int side : range(5)){
                    neighbor += numberOfCellsOnSide(side);
                }
                neighbor += j*block.numberOfCells_X() + i;
                return neighbor;
            }
            break;
        default:
            std::domain_error("FaceId out of bounds for faceNeighbor.");

    }

    return block.convert_ijk_ToCellId(i, j, k);
}

bool CartMesh::isFaceBoundary(int cellId, int faceId) const 
{
    int neighbor = faceNeighbor(cellId, faceId);
    if(neighbor >= numberOfCells()){
        return true;
    }
    else {
        return false;
    }
}

double CartMesh::cellVolume(int cellId) const 
{
    return volume;
}

Point<double> CartMesh::cellCentroid(int cellId) const
{
    int i,j,k;

    block.convertCellIdTo_ijk(cellId, i, j, k);
    int loNodeId = block.convert_ijk_ToNodeId(i,j,k);
    int hiNodeId = block.convert_ijk_ToNodeId(i+1, j+1,k+1);

    double lo[3], hi[3];
    getNode(loNodeId, lo);
    getNode(hiNodeId, hi);

    return Point<double>(
            0.5*(lo[0] + hi[0]), 
            0.5*(lo[1] + hi[1]), 
            0.5*(lo[2] + hi[2])
            );
}

Point<double> CartMesh::faceArea(int cellId, int faceId) const
{
    double area;
    switch (faceId) {
        case 0:
            area = block.get_dx() * block.get_dy();
            return {0,0,-area};
        case 1:
            area = block.get_dx() * block.get_dz();
            return {0, -area, 0};
        case 2:
            area = block.get_dy() * block.get_dz();
            return {area, 0, 0};
        case 3:
            area = block.get_dx() * block.get_dz();
            return {0,  area, 0};
        case 4:
            area = block.get_dy() * block.get_dz();
            return {-area, 0, 0};
        case 5:
            area = block.get_dx() * block.get_dy();
            return {0,0,area};
        default:
            throw std::domain_error("FaceId out of bounds for faceArea.");
    };
}

std::vector<int> CartMesh::getNodesInCellFace(int cellId, int faceId) const
{
    int i, j, k;
    block.convertCellIdTo_ijk(cellId, i, j, k);

    int nodes[8];
    nodes[0] = block.convert_ijk_ToNodeId(i  , j  , k  );
    nodes[1] = block.convert_ijk_ToNodeId(i+1, j  , k  );
    nodes[2] = block.convert_ijk_ToNodeId(i  , j+1, k  );
    nodes[3] = block.convert_ijk_ToNodeId(i+1, j+1, k  );
    nodes[4] = block.convert_ijk_ToNodeId(i  , j  , k+1);
    nodes[5] = block.convert_ijk_ToNodeId(i+1, j  , k+1);
    nodes[6] = block.convert_ijk_ToNodeId(i  , j+1, k+1);
    nodes[7] = block.convert_ijk_ToNodeId(i+1, j+1, k+1);

    std::vector<int> faceNodes(4);
    switch (faceId) {
        case 0:
            faceNodes[0] = nodes[0];
            faceNodes[1] = nodes[2];
            faceNodes[2] = nodes[3];
            faceNodes[3] = nodes[1];
            return faceNodes;
        case 1:
            faceNodes[0] = nodes[0];
            faceNodes[1] = nodes[1];
            faceNodes[2] = nodes[5];
            faceNodes[3] = nodes[4];
            return faceNodes;
        case 2:
            faceNodes[0] = nodes[3];
            faceNodes[1] = nodes[7];
            faceNodes[2] = nodes[5];
            faceNodes[3] = nodes[1];
            return faceNodes;
        case 3:
            faceNodes[0] = nodes[2];
            faceNodes[1] = nodes[6];
            faceNodes[2] = nodes[7];
            faceNodes[3] = nodes[3];
            return faceNodes;
        case 4:
            faceNodes[0] = nodes[2];
            faceNodes[1] = nodes[0];
            faceNodes[2] = nodes[4];
            faceNodes[3] = nodes[6];
            return faceNodes;
        case 5:
            faceNodes[0] = nodes[7];
            faceNodes[1] = nodes[6];
            faceNodes[2] = nodes[4];
            faceNodes[3] = nodes[5];
            return faceNodes;
        default:
            throw std::domain_error("Requested nodes for face " + std::to_string(faceId) + " but cart mesh cells only have 6 faces.");
    }
}

std::vector<int> CartMesh::getNodesInCell(int cellId) const
{
    int i, j, k;
    block.convertCellIdTo_ijk(cellId, i, j, k);

    std::vector<int> cellNodes(8);
    cellNodes[0] = block.convert_ijk_ToNodeId(i  , j  , k  );
    cellNodes[1] = block.convert_ijk_ToNodeId(i+1, j  , k  );
    cellNodes[2] = block.convert_ijk_ToNodeId(i  , j+1, k  );
    cellNodes[3] = block.convert_ijk_ToNodeId(i+1, j+1, k  );
    cellNodes[4] = block.convert_ijk_ToNodeId(i  , j  , k+1);
    cellNodes[5] = block.convert_ijk_ToNodeId(i+1, j  , k+1);
    cellNodes[6] = block.convert_ijk_ToNodeId(i  , j+1, k+1);
    cellNodes[7] = block.convert_ijk_ToNodeId(i+1, j+1, k+1);
    return cellNodes;
}

int CartMesh::numberOfCellsOnSide(int side) const
{
    switch (side) {
        // z-plane faces
        case 0:
        case 5:
            return block.numberOfCells_X() * block.numberOfCells_Y();
            // y-plane faces
        case 1:
        case 3:
            return block.numberOfCells_X() * block.numberOfCells_Z();
            // x-plane faces
        case 2:
        case 4:
            return block.numberOfCells_Y() * block.numberOfCells_Z();
        default:
            throw std::domain_error("Side out of bounds for numberOfCellsOnSide(int)");
    }
}

int CartMesh::faceBoundaryTag(int cellId, int faceId) const
{
    return whatSideIsCellFaceOn(cellId, faceId);
}

int CartMesh::whatSideIsCellFaceOn(int cellId, int faceId) const
{
    int i, j, k;
    block.convertCellIdTo_ijk(cellId, i, j, k);

    switch (faceId) {
        case 0:
            if(k == 0)
                return 0;
            break;
        case 1:
            if(j == 0)
                return 1;
            break;
        case 2:
            if(i == block.numberOfCells_X())
                return 2;
            break;
        case 3:
            if(j == block.numberOfCells_Y())
                return 3;
            break;
        case 4:
            if(i == 0)
                return 4;
            break;
        case 5:
            if(k == block.numberOfCells_Z())
                return 5;
            break;
        default:
            return -1;
    }
    return -1;
}


















