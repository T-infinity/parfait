#include "catch.hpp"
#include <parfait/Edges.h>
#include <parfait/CGNSElements.h>
#include <parfait/CartMesh.h>

TEST_CASE("GenericMeshToolsTests, UniqueEdges"){

    Parfait::CartMesh cartMesh({0,0,0}, {1,1,1}, 1,1,1);
    auto edges = Parfait::Edges::getUniqueEdges(cartMesh);
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

TEST_CASE("add cells to edge list"){
    Parfait::Edges::EdgeBuilder edgeBuilder;
    std::array<int,4> tet {9,5,30,32};
    edgeBuilder.addCell(tet.data(),Parfait::CGNS::Tet::edge_to_node);

    auto edges = edgeBuilder.edges();

    REQUIRE(6 == edges.size());
    REQUIRE(5 == edges.front()[0]);
    REQUIRE(9 == edges.front()[1]);
}

TEST_CASE("add cells to edge list"){
    Parfait::Edges::EdgeBuilder edgeBuilder;
    std::array<int,4> tet {9,5,30,32};
    edgeBuilder.addCell(tet.data(),Parfait::CGNS::Tet::edge_to_node);

    auto edges = edgeBuilder.edges();

    REQUIRE(6 == edges.size());
    REQUIRE(5 == edges.front()[0]);
    REQUIRE(9 == edges.front()[1]);
}
