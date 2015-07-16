#include "GenericMesh.h"
#include "CartMesh.h"
#include "GenericMeshTools.h"
#include "CppUTest/CommandLineTestRunner.h"
using namespace Parfait;

TEST_GROUP(GenericMeshToolsTests)
{
    double tol;
	void setup()
	{
        tol = 1.0e-14;
	}
	void teardown()
	{
	}
};

TEST(GenericMeshToolsTests,CellCenter)
{
    CartMesh cartMesh({0,0,2}, {1,2,3}, 1, 1, 1);

    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    auto center = GenericMeshTools::cellCenter(mesh, cell);
    DOUBLES_EQUAL(0.5, center[0], tol);
    DOUBLES_EQUAL(1.0, center[1], tol);
    DOUBLES_EQUAL(2.5, center[2], tol);
}

TEST(GenericMeshToolsTests,CellVolume)
{
    CartMesh cartMesh({0,0,2}, {1,1,3}, 2, 2, 2);

    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    double volume = GenericMeshTools::computeCellVolume(mesh, cell);
    DOUBLES_EQUAL(0.125, volume, tol);
}

TEST(GenericMeshToolsTests, FaceCenter)
{
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);
    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    auto face = cell.getFace(0);
    auto faceCenter = GenericMeshTools::faceCenter(mesh, face);
    DOUBLES_EQUAL(0.25, faceCenter[0], tol);
    DOUBLES_EQUAL(0.25, faceCenter[1], tol);
    DOUBLES_EQUAL(   0, faceCenter[2], tol);
}

TEST(GenericMeshToolsTests, FaceArea)
{
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);
    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    auto face = cell.getFace(0);
    auto areaVector = GenericMeshTools::getFaceArea(mesh, face);
    DOUBLES_EQUAL(0,    areaVector[0], tol);
    DOUBLES_EQUAL(0,    areaVector[1], tol);
    DOUBLES_EQUAL(-.25, areaVector[2], tol);
}

TEST(GenericMeshToolsTests, node2Cell)
{
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);

    auto node2Cell = GenericMeshTools::buildNode2Cell(cartMesh);

    LONGS_EQUAL(27, node2Cell.size());
    //--- bottom face nodes
    LONGS_EQUAL(1, node2Cell[0].size());
    LONGS_EQUAL(2, node2Cell[1].size());
    LONGS_EQUAL(1, node2Cell[2].size());
    LONGS_EQUAL(2, node2Cell[3].size());
    LONGS_EQUAL(4, node2Cell[4].size());
    LONGS_EQUAL(2, node2Cell[5].size());
    LONGS_EQUAL(1, node2Cell[6].size());
    LONGS_EQUAL(2, node2Cell[7].size());
    LONGS_EQUAL(1, node2Cell[8].size());

    //--- middle face nodes
    LONGS_EQUAL(2, node2Cell[ 9].size());
    LONGS_EQUAL(4, node2Cell[10].size());
    LONGS_EQUAL(2, node2Cell[11].size());
    LONGS_EQUAL(4, node2Cell[12].size());
    LONGS_EQUAL(8, node2Cell[13].size());
    LONGS_EQUAL(4, node2Cell[14].size());
    LONGS_EQUAL(2, node2Cell[15].size());
    LONGS_EQUAL(4, node2Cell[16].size());
    LONGS_EQUAL(2, node2Cell[17].size());

    //--- top face nodes
    LONGS_EQUAL(1, node2Cell[18].size());
    LONGS_EQUAL(2, node2Cell[19].size());
    LONGS_EQUAL(1, node2Cell[20].size());
    LONGS_EQUAL(2, node2Cell[21].size());
    LONGS_EQUAL(4, node2Cell[22].size());
    LONGS_EQUAL(2, node2Cell[23].size());
    LONGS_EQUAL(1, node2Cell[24].size());
    LONGS_EQUAL(2, node2Cell[25].size());
    LONGS_EQUAL(1, node2Cell[26].size());
}

TEST(GenericMeshToolsTests, UniqueEdges){

    CartMesh cartMesh({0,0,0}, {1,1,1}, 1,1,1);
    auto edges = GenericMeshTools::getUniqueEdges(cartMesh);
    LONGS_EQUAL(12, edges.size());

    std::vector<std::array<int,2>> validEdges(12);
    validEdges[0]  = std::array<int, 2>{0,1};
    validEdges[1]  = std::array<int, 2>{0,2};
    validEdges[2]  = std::array<int, 2>{0,4};
    validEdges[3]  = std::array<int, 2>{1,3};
    validEdges[4]  = std::array<int, 2>{1,5};
    validEdges[5]  = std::array<int, 2>{2,3};
    validEdges[6]  = std::array<int, 2>{2,6};
    validEdges[7]  = std::array<int, 2>{3,7};
    validEdges[8]  = std::array<int, 2>{4,5};
    validEdges[9]  = std::array<int, 2>{4,6};
    validEdges[10] = std::array<int, 2>{5,7};
    validEdges[11] = std::array<int, 2>{6,7};

    int index = 0;
    for(auto edge : edges){
        CHECK(validEdges[index++] == edge);
    }
};
