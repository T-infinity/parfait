#include <catch.hpp>
#include <CartBlock.h>
#include <VtkUnstructuredWriter.h>

TEST_CASE("Dynamic Vtk"){
    Parfait::CartBlock mesh({{0,0,0},{1,1,1}}, 5,5,8);

    Parfait::VtkUnstructuredWriter writer("some-block", mesh);
    writer.writeBinary();
}