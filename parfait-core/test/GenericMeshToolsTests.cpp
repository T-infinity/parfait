#include "GenericMesh.h"
#include "CartMesh.h"
#include "GenericMeshTools.h"
#include <catch.hpp>
using namespace Parfait;

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
