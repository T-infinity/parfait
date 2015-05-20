#include "cart_mesh.h"
#include "vector_tools.h"

#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(CartMesh)
{
};

TEST(CartMesh, Exists)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    CHECK_EQUAL(8, mesh.numberOfCells());

    CHECK_EQUAL(24, mesh.numberOfBoundaryFaces());

}

TEST(CartMesh, Centroid){

    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    Point centroid = mesh.cellCentroid(0);
    double tol = 1.0e-14;
    DOUBLES_EQUAL(0.25, centroid[0], tol);
    DOUBLES_EQUAL(0.25, centroid[1], tol);
    DOUBLES_EQUAL(0.25, centroid[2], tol);

    centroid = mesh.cellCentroid(1);
    DOUBLES_EQUAL(0.75, centroid[0], tol);
    DOUBLES_EQUAL(0.25, centroid[1], tol);
    DOUBLES_EQUAL(0.25, centroid[2], tol);
}

TEST(CartMesh, GetCellNodes)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    auto cellNodes = mesh.getNodesInCell(0);
    CHECK_EQUAL(8, cellNodes.size());

    CHECK_EQUAL(true, isIn(cellNodes, 0));
    CHECK_EQUAL(true, isIn(cellNodes, 1));
    CHECK_EQUAL(true, isIn(cellNodes, 3));
    CHECK_EQUAL(true, isIn(cellNodes, 4));
    CHECK_EQUAL(true, isIn(cellNodes, 9));
    CHECK_EQUAL(true, isIn(cellNodes, 10));
    CHECK_EQUAL(true, isIn(cellNodes, 12));
    CHECK_EQUAL(true, isIn(cellNodes, 13));
}

TEST(CartMesh, GetFaceNodes)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    std::vector<int> faceNodes = mesh.getNodesInCellFace(0,0);

    CHECK_EQUAL(true, isIn(faceNodes, 0));
    CHECK_EQUAL(true, isIn(faceNodes, 1));
    CHECK_EQUAL(true, isIn(faceNodes, 3));
    CHECK_EQUAL(true, isIn(faceNodes, 4));

    faceNodes = mesh.getNodesInCellFace(0, 1);
    
    CHECK_EQUAL(true, isIn(faceNodes, 0));
    CHECK_EQUAL(true, isIn(faceNodes, 1));
    CHECK_EQUAL(true, isIn(faceNodes, 9));
    CHECK_EQUAL(true, isIn(faceNodes, 10));

    faceNodes = mesh.getNodesInCellFace(0, 2);
    CHECK_EQUAL(true, isIn(faceNodes, 1));
    CHECK_EQUAL(true, isIn(faceNodes, 4));
    CHECK_EQUAL(true, isIn(faceNodes, 10));
    CHECK_EQUAL(true, isIn(faceNodes, 13));

    faceNodes = mesh.getNodesInCellFace(0, 3);
    CHECK_EQUAL(true, isIn(faceNodes, 3));
    CHECK_EQUAL(true, isIn(faceNodes, 4));
    CHECK_EQUAL(true, isIn(faceNodes, 12));
    CHECK_EQUAL(true, isIn(faceNodes, 13));

    faceNodes = mesh.getNodesInCellFace(0, 4);
    CHECK_EQUAL(true, isIn(faceNodes, 0));
    CHECK_EQUAL(true, isIn(faceNodes, 3));
    CHECK_EQUAL(true, isIn(faceNodes, 9));
    CHECK_EQUAL(true, isIn(faceNodes, 12));

    faceNodes = mesh.getNodesInCellFace(0, 5);
    CHECK_EQUAL(true, isIn(faceNodes, 9));
    CHECK_EQUAL(true, isIn(faceNodes, 10));
    CHECK_EQUAL(true, isIn(faceNodes, 12));
    CHECK_EQUAL(true, isIn(faceNodes, 13));
}

TEST(CartMesh, PlotMesh)
{

    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);
}

TEST(CartMesh, FaceArea)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    double area = 0.25;
    double tol = 1.0e-14;

    Point faceArea = mesh.faceArea(0,0);
    DOUBLES_EQUAL(0    , faceArea[0], tol);
    DOUBLES_EQUAL(0    , faceArea[1], tol);
    DOUBLES_EQUAL(-area, faceArea[2], tol);

    faceArea = mesh.faceArea(1, 0);
    DOUBLES_EQUAL(0    , faceArea[0], tol);
    DOUBLES_EQUAL(0    , faceArea[1], tol);
    DOUBLES_EQUAL(-area, faceArea[2], tol);

    faceArea = mesh.faceArea(0, 1);
    DOUBLES_EQUAL(0    , faceArea[0], tol);
    DOUBLES_EQUAL(-area, faceArea[1], tol);
    DOUBLES_EQUAL(0    , faceArea[2], tol);

    faceArea = mesh.faceArea(0, 2);
    DOUBLES_EQUAL(area , faceArea[0], tol);
    DOUBLES_EQUAL(0    , faceArea[1], tol);
    DOUBLES_EQUAL(0    , faceArea[2], tol);

    faceArea = mesh.faceArea(0, 3);
    DOUBLES_EQUAL(0    , faceArea[0], tol);
    DOUBLES_EQUAL(area , faceArea[1], tol);
    DOUBLES_EQUAL(0    , faceArea[2], tol);

    faceArea = mesh.faceArea(0, 4);
    DOUBLES_EQUAL(-area, faceArea[0], tol);
    DOUBLES_EQUAL(0    , faceArea[1], tol);
    DOUBLES_EQUAL(0    , faceArea[2], tol);

    faceArea = mesh.faceArea(0, 5);
    DOUBLES_EQUAL(0    , faceArea[0], tol);
    DOUBLES_EQUAL(0    , faceArea[1], tol);
    DOUBLES_EQUAL(area , faceArea[2], tol);
}

TEST(CartMesh, Neighbors)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    // Check internal neighbors
    int neighbor = mesh.faceNeighbor(0,2);
    CHECK_EQUAL(1, neighbor);

    neighbor = mesh.faceNeighbor(1, 4);
    CHECK_EQUAL(0, neighbor);

    neighbor = mesh.faceNeighbor(0, 3);
    CHECK_EQUAL(2, neighbor);

    neighbor = mesh.faceNeighbor(3, 1);
    CHECK_EQUAL(1, neighbor);
    neighbor = mesh.faceNeighbor(3, 5);
    CHECK_EQUAL(7, neighbor);


    // Check boundary neighbors.
    // side 0
    neighbor = mesh.faceNeighbor(0, 0);
    CHECK_EQUAL(8, neighbor);
    neighbor = mesh.faceNeighbor(1, 0);
    CHECK_EQUAL(9, neighbor);
    neighbor = mesh.faceNeighbor(2, 0);
    CHECK_EQUAL(10, neighbor);
    neighbor = mesh.faceNeighbor(3, 0);
    CHECK_EQUAL(11, neighbor);

    // side 1
    neighbor = mesh.faceNeighbor(0, 1);
    CHECK_EQUAL(12, neighbor);
    neighbor = mesh.faceNeighbor(1, 1);
    CHECK_EQUAL(13, neighbor);
    neighbor = mesh.faceNeighbor(4, 1);
    CHECK_EQUAL(14, neighbor);
    neighbor = mesh.faceNeighbor(5, 1);
    CHECK_EQUAL(15, neighbor);

    // side 2
    neighbor = mesh.faceNeighbor(1, 2);
    CHECK_EQUAL(16, neighbor);
    neighbor = mesh.faceNeighbor(3, 2);
    CHECK_EQUAL(17, neighbor);
    neighbor = mesh.faceNeighbor(5, 2);
    CHECK_EQUAL(18, neighbor);
    neighbor = mesh.faceNeighbor(7, 2);
    CHECK_EQUAL(19, neighbor);

    // side 3
    neighbor = mesh.faceNeighbor(2, 3);
    CHECK_EQUAL(20, neighbor);
    neighbor = mesh.faceNeighbor(3, 3);
    CHECK_EQUAL(21, neighbor);
    neighbor = mesh.faceNeighbor(6, 3);
    CHECK_EQUAL(22, neighbor);
    neighbor = mesh.faceNeighbor(7, 3);
    CHECK_EQUAL(23, neighbor);

    // side 4
    neighbor = mesh.faceNeighbor(0, 4);
    CHECK_EQUAL(24, neighbor);
    neighbor = mesh.faceNeighbor(2, 4);
    CHECK_EQUAL(25, neighbor);
    neighbor = mesh.faceNeighbor(4, 4);
    CHECK_EQUAL(26, neighbor);
    neighbor = mesh.faceNeighbor(6, 4);
    CHECK_EQUAL(27, neighbor);

    // side 5
    neighbor = mesh.faceNeighbor(4, 5);
    CHECK_EQUAL(28, neighbor);
    neighbor = mesh.faceNeighbor(5, 5);
    CHECK_EQUAL(29, neighbor);
    neighbor = mesh.faceNeighbor(6, 5);
    CHECK_EQUAL(30, neighbor);
    neighbor = mesh.faceNeighbor(7, 5);
    CHECK_EQUAL(31, neighbor);

}

TEST(CartMesh, isBoundary)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    bool isBoundary = mesh.isFaceBoundary(0, 0);
    CHECK_EQUAL(true, isBoundary);

    isBoundary = mesh.isFaceBoundary(0, 2);
    CHECK_EQUAL(false, isBoundary);
}

int myCoolTest(){
    
    printf("\nWoo!");
    return 98;
}

TEST(CartMesh, BoundaryTag)
{
    Point lo(0,0,0);
    Point hi(1,1,1);
    int numCells = 2;
    CartMesh mesh(lo, hi, numCells, numCells, numCells);

    int cellId = 0; int faceId = 0;
    //int tag = mesh.faceBoundaryTag(0,0);


    LONGS_EQUAL( 0, mesh.faceBoundaryTag(cellId, 0));
    LONGS_EQUAL( 1, mesh.faceBoundaryTag(cellId, 1));
    LONGS_EQUAL(-1, mesh.faceBoundaryTag(cellId, 2));
    LONGS_EQUAL(-1, mesh.faceBoundaryTag(cellId, 3));
    LONGS_EQUAL( 4, mesh.faceBoundaryTag(cellId, 4));
    LONGS_EQUAL(-1, mesh.faceBoundaryTag(cellId, 5));
}











