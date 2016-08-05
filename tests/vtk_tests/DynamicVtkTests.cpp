#include <catch.hpp>
#include <CartBlock.h>
#include <VtkUnstructuredWriter.h>
#include <VolumeMeshAdapter.h>

TEST_CASE("Dynamic Vtk"){
    Parfait::CartBlock mesh({{0,0,0},{1,1,1}}, 5,5,8);
    Parfait::VolumeMeshAdapter adapter(mesh);

    Parfait::VtkUnstructuredWriter writer("some-adapted-block", adapter);
    writer.writeBinary();
}