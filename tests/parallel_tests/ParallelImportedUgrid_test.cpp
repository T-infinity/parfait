#include <ParallelImportedUgrid.h>
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(ParallelImportedUgrid){};

TEST(ParallelImportedUgrid, Exists) {
    Parfait::ParallelImportedUgrid mesh;
    mesh.globalNumberOfNodes();
}
