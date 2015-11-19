#include <catch.hpp>
#include "VtkUnstructuredSurfaceWriter.h"

class MockVtkSurfaceMesh{
public:
    int numberOfBoundaryFaces(){return 2;}
};

TEST_CASE("can create a vizualization for a surface mesh"){
    MockVtkSurfaceMesh mesh;
    Parfait::VtkUnstructuredSurfaceWriter("surface",mesh);
}

