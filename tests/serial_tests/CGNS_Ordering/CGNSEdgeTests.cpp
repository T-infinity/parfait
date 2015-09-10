#include <catch.hpp>
#include <CGNS.h>

TEST_CASE("Tet edge windings"){
    std::array<int,4> tet = {11,12,13,14};
    auto n = Parfait::CGNS::Edges::Tet::numberOfEdges();
    REQUIRE(n == 6);
    auto e = Parfait::CGNS::Edges::Tet::getEdge(tet, 0);
    REQUIRE(e[0] == 11);
    REQUIRE(e[1] == 12);
}

