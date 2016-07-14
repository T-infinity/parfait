#include "GenericMesh.h"
#include "CartMesh.h"
#include "GenericMeshTools.h"
#include <catch.hpp>
using namespace Parfait;

TEST_CASE("GenericMeshToolsTests,CellCenter")
{
    CartMesh cartMesh({0,0,2}, {1,2,3}, 1, 1, 1);

    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    auto center = GenericMeshTools::cellCenter(mesh, cell);
    REQUIRE(0.5 == Approx(center[0]));
    REQUIRE(1.0 == Approx(center[1]));
    REQUIRE(2.5 == Approx(center[2]));
}

TEST_CASE("GenericMeshToolsTests,CellVolume")
{
    CartMesh cartMesh({0,0,2}, {1,1,3}, 2, 2, 2);

    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    double volume = GenericMeshTools::computeCellVolume(mesh, cell);
    REQUIRE(0.125 == Approx(volume));
}

TEST_CASE("GenericMeshToolsTests, FaceCenter")
{
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);
    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    auto face = cell.getFace(0);
    auto faceCenter = GenericMeshTools::faceCenter(mesh, face);
    REQUIRE(0.25 == Approx(faceCenter[0]));
    REQUIRE(0.25 == Approx(faceCenter[1]));
    REQUIRE(   0 == Approx(faceCenter[2]));
}

TEST_CASE("GenericMeshToolsTests, FaceArea")
{
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);
    Mesh<CartMesh> mesh(cartMesh);

    auto cell = mesh.cell(0);
    auto face = cell.getFace(0);
    auto areaVector = GenericMeshTools::getFaceArea(mesh, face);
    REQUIRE(0 ==    Approx(areaVector[0]));
    REQUIRE(0 ==    Approx(areaVector[1]));
    REQUIRE(-.25 == Approx(areaVector[2]));
}

TEST_CASE("GenericMeshToolsTests, node2Cell")
{
    CartMesh cartMesh({0,0,0}, {1,1,1}, 2, 2, 2);

    auto node2Cell = GenericMeshTools::buildNode2Cell(cartMesh);

    REQUIRE(27 == node2Cell.size());
    //--- bottom face nodes
    REQUIRE(1 == node2Cell[0].size());
    REQUIRE(2 == node2Cell[1].size());
    REQUIRE(1 == node2Cell[2].size());
    REQUIRE(2 == node2Cell[3].size());
    REQUIRE(4 == node2Cell[4].size());
    REQUIRE(2 == node2Cell[5].size());
    REQUIRE(1 == node2Cell[6].size());
    REQUIRE(2 == node2Cell[7].size());
    REQUIRE(1 == node2Cell[8].size());

    //--- middle face nodes
    REQUIRE(2 == node2Cell[ 9].size());
    REQUIRE(4 == node2Cell[10].size());
    REQUIRE(2 == node2Cell[11].size());
    REQUIRE(4 == node2Cell[12].size());
    REQUIRE(8 == node2Cell[13].size());
    REQUIRE(4 == node2Cell[14].size());
    REQUIRE(2 == node2Cell[15].size());
    REQUIRE(4 == node2Cell[16].size());
    REQUIRE(2 == node2Cell[17].size());

    //--- top face nodes
    REQUIRE(1 == node2Cell[18].size());
    REQUIRE(2 == node2Cell[19].size());
    REQUIRE(1 == node2Cell[20].size());
    REQUIRE(2 == node2Cell[21].size());
    REQUIRE(4 == node2Cell[22].size());
    REQUIRE(2 == node2Cell[23].size());
    REQUIRE(1 == node2Cell[24].size());
    REQUIRE(2 == node2Cell[25].size());
    REQUIRE(1 == node2Cell[26].size());
}

TEST_CASE("GenericMeshToolsTests, UniqueEdges"){

    CartMesh cartMesh({0,0,0}, {1,1,1}, 1,1,1);
    auto edges = GenericMeshTools::getUniqueEdges(cartMesh);
    REQUIRE(12 == edges.size());

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
        REQUIRE(validEdges[index++] == edge);
    }
};
