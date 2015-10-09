#include "MeshConnectivity.h"

#include "catch.hpp"

using namespace Parfait;

TEST_CASE("test getters"){
    MeshConnectivity connectivity;

    SECTION("for tets") {
        connectivity.tets = {0, 1, 2, 3};
        REQUIRE(1 == connectivity.numberOfTets());
        int *p = connectivity.getTetPtr(0);
        for(int i=0;i<4;i++)
            REQUIRE(i == p[i]);
    }
    SECTION("for pyramids"){
        connectivity.pyramids = {0,1,2,3,4};
        REQUIRE(1 == connectivity.numberOfPyramids());
        int *p = connectivity.getPyramidPtr(0);
        for(int i=0;i<5;i++)
            REQUIRE(i == p[i]);
    }

    SECTION("for prisms"){
        connectivity.prisms = {0,1,2,3,4,5};
        REQUIRE(1 == connectivity.numberOfPrisms());
        int *p = connectivity.getPrismPtr(0);
        for(int i=0;i<6;i++)
            REQUIRE(i == p[i]);
    }

    SECTION("for hexs"){
        connectivity.hexes = {0,1,2,3,4,5,6,7};
        REQUIRE(1 == connectivity.numberOfHexes());
        int *p = connectivity.getHexPtr(0);
        for(int i=0;i<8;i++)
            REQUIRE(i == p[i]);
    }

    SECTION("for triangles"){
        connectivity.triangles = {0,1,2};
        REQUIRE(1 == connectivity.numberOfTriangles());
        int *p = connectivity.getTrianglePtr(0);
        for(int i=0;i<3;i++)
            REQUIRE(i == p[i]);
    }

    SECTION("for quads"){
        connectivity.quads = {0,1,2,3};
        REQUIRE(1 == connectivity.numberOfQuads());
        int *p = connectivity.getQuadPtr(0);
        for(int i=0;i<4;i++)
            REQUIRE(i == p[i]);
    }

    SECTION("For generic style access of volume elements"){
        connectivity.tets = {0, 1, 2, 3};
        connectivity.pyramids = {4,5,6,7,8};
        connectivity.prisms = {0,9,8,7,6,5};
        connectivity.hexes = {5,4,7,1,4,3,2,9};

        REQUIRE(4 == connectivity.numberOfCells());

        REQUIRE(4 == connectivity.sizeOfCell(0));
        REQUIRE(5 == connectivity.sizeOfCell(1));
        REQUIRE(6 == connectivity.sizeOfCell(2));
        REQUIRE(8 == connectivity.sizeOfCell(3));

        SECTION("check tet") {
            int *p = connectivity.getCellPtr(0);
            REQUIRE(0 == p[0]);
            REQUIRE(1 == p[1]);
            REQUIRE(2 == p[2]);
            REQUIRE(3 == p[3]);
        }

        SECTION("check pyramid") {
            int *p = connectivity.getCellPtr(1);
            REQUIRE(4 == p[0]);
            REQUIRE(5 == p[1]);
            REQUIRE(6 == p[2]);
            REQUIRE(7 == p[3]);
            REQUIRE(8 == p[4]);
        }

        SECTION("check prism") {
            int *p = connectivity.getCellPtr(2);
            REQUIRE(0 == p[0]);
            REQUIRE(9 == p[1]);
            REQUIRE(8 == p[2]);
            REQUIRE(7 == p[3]);
            REQUIRE(6 == p[4]);
            REQUIRE(5 == p[5]);
        }

        SECTION("check hex") {
            int *p = connectivity.getCellPtr(3);
            REQUIRE(5 == p[0]);
            REQUIRE(4 == p[1]);
            REQUIRE(7 == p[2]);
            REQUIRE(1 == p[3]);
            REQUIRE(4 == p[4]);
            REQUIRE(3 == p[5]);
            REQUIRE(2 == p[6]);
            REQUIRE(9 == p[7]);
        }
    }
}

