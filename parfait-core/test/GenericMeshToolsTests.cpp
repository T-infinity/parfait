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

TEST_CASE("GenericMeshToolsTests, UniqueEdges"){

    CartMesh cartMesh({0,0,0}, {1,1,1}, 1,1,1);
    auto edges = GenericMeshTools::getUniqueEdges(cartMesh);
    REQUIRE(12 == edges.size());

    std::vector<std::array<int,2>> validEdges(12);
    validEdges[0]  = std::array<int, 2>{0,1};
    validEdges[1]  = std::array<int, 2>{0,3};
    validEdges[2]  = std::array<int, 2>{0,4};
    validEdges[3]  = std::array<int, 2>{1,2};
    validEdges[4]  = std::array<int, 2>{1,5};
    validEdges[5]  = std::array<int, 2>{2,3};
    validEdges[6]  = std::array<int, 2>{2,6};
    validEdges[7]  = std::array<int, 2>{3,7};
    validEdges[8]  = std::array<int, 2>{4,5};
    validEdges[9]  = std::array<int, 2>{4,7};
    validEdges[10]  = std::array<int, 2>{5,6};
    validEdges[11]  = std::array<int, 2>{6,7};

    int index = 0;
    for(auto edge : edges){
        REQUIRE(validEdges[index++] == edge);
    }
};
