#include <catch.hpp>
#include "CartMesh.h"
#include "VtkExplodedViewWriter.h"
#include "VtkGenericWriter.h"
#include "ExplodedMesh.h"

TEST_CASE("VtkExplodedView, Exists"){
    Parfait::CartMesh mesh({0,0,0},{1,1,1}, 10,10,10);
    Parfait::VtkExplodedViewWriter<Parfait::CartMesh> writer(mesh, "exploded");
    writer.writeAscii();
}

TEST_CASE("ExplodedMesh, Exists"){
    Parfait::CartMesh mesh({0,0,0},{1,1,1}, 2,2,2);
    Parfait::ExplodedMesh explodedMesh(mesh);
    CHECK(explodedMesh.numberOfCells() == mesh.numberOfCells());
    CHECK(explodedMesh.numberOfNodes() == 64);
    Parfait::VtkWriter<Parfait::ExplodedMesh> writer(explodedMesh, "exploded-copy");
    writer.writeAscii();
}


