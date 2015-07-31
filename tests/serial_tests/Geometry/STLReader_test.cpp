#include <STL.h>
#include <STLReader.h>
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(STLReader){};

TEST(STLReader, Exists){
    Parfait::STL::Reader reader("TEST.stl");
    LONGS_EQUAL(12, reader.numberOfFacets());
    auto facets = reader.readFacets(0,6);
    LONGS_EQUAL(6, facets.size());
    Parfait::STL::STL stl1;
    stl1.facets = reader.readFacets(0,6);
    Parfait::STL::STL stl2;
    stl2.facets = reader.readFacets(6,12);
    stl1.writeAsciiFile("stl1");
    stl2.writeAsciiFile("stl2");
}
