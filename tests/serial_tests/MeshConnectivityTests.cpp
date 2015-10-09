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
}

