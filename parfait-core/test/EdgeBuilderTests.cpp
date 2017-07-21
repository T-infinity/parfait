#include "catch.hpp"
#include <EdgeBuilder.h>
#include <CGNSElements.h>

TEST_CASE("add cells to edge list"){
    Parfait::EdgeBuilder edgeBuilder;
    std::array<int,4> tet {9,5,30,32};
    edgeBuilder.addCell(tet.data(),Parfait::CGNS::Tet::edge_to_node);

    auto edges = edgeBuilder.edges();

    REQUIRE(6 == edges.size());
    REQUIRE(5 == edges.front()[0]);
    REQUIRE(9 == edges.front()[1]);
}
