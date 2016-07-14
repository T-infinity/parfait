#include <STL.h>
#include <STLReader.h>
#include <catch.hpp>


TEST_CASE("STLReader, Exists"){
    // TODO: add stl to repo.
#if 0
    Parfait::STL::Reader reader("TEST.stl");
    REQUIRE(12 == reader.numberOfFacets());
    auto facets = reader.readFacets(0,6);
    REQUIRE(6 == facets.size());
    Parfait::STL::STL stl1;
    stl1.facets = reader.readFacets(0,6);
    Parfait::STL::STL stl2;
    stl2.facets = reader.readFacets(6,12);
    stl1.writeAsciiFile("stl1");
    stl2.writeAsciiFile("stl2");
#endif
}
