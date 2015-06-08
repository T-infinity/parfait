
#include "mesh_slice.h"
#include "extent.h"
#include "cart_mesh.h"
#include "generic_mesh.h"
#include "vtk_slice_writer.h"
#include "vtk_generic_writer.h"
#include "generic_mesh_tools.h"

#include "CppUTest/CommandLineTestRunner.h"
using namespace Parfait;
TEST_GROUP(VtkSliceWriter) {

};

TEST(VtkSliceWriter, Exists) {
    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);

    Extent domain({.49, 0, 0}, {.49, 1, 1});
}

TEST(VtkSliceWriter, CellCenterData_double){

    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);
    Extent domain({.49, 0, 0}, {.49, 1, 1});

    std::vector<double> Q(cartMesh.numberOfCells(), -1);
    Mesh<CartMesh> genMesh(cartMesh);
    for (auto cell : genMesh.cells()) {
        Point center = GenericMeshTools::cellCenter(genMesh, cell);
        Q[cell.Id()] = center[0] + center[1] + center[2];
    }

    {
        VtkWriter<CartMesh> writer(cartMesh, "full-cell-double");
        writer.addScalarField(false, "function", Q.data());
        writer.writeAscii();
    }

    {
        VtkSliceWriter<CartMesh> writer(cartMesh, domain, "slice-cell-double");
        writer.addScalarField(false, "function", Q.data());
        writer.writeAscii();
    }
}

TEST(VtkSliceWriter, CellCenterData_float){

    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);
    Extent domain({.49, 0, 0}, {.49, 1, 1});

    std::vector<float> Q(cartMesh.numberOfCells(), -1);
    Mesh<CartMesh> genMesh(cartMesh);
    for (auto cell : genMesh.cells()) {
        Point center = GenericMeshTools::cellCenter(genMesh, cell);
        Q[cell.Id()] = center[0] + center[1] + center[2];
    }

    {
        VtkWriter<CartMesh> writer(cartMesh, "full-cell-float");
        writer.addScalarField(false, "function", Q.data());
        writer.writeAscii();
    }

    {
        VtkSliceWriter<CartMesh> writer(cartMesh, domain, "slice-cell-float");
        writer.addScalarField(false, "function", Q.data());
        writer.writeAscii();
    }
}

TEST(VtkSliceWriter, CellCenterData_int){

    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);
    Extent domain({.49, 0, 0}, {.49, 1, 1});

    std::vector<int> Q(cartMesh.numberOfCells(), -1);
    Mesh<CartMesh> genMesh(cartMesh);
    for (auto cell : genMesh.cells()) {
        Q[cell.Id()] = cell.Id();
    }

    {
        VtkWriter<CartMesh> writer(cartMesh, "full-cell-int");
        writer.addScalarField(false, "function", Q.data());
        writer.writeAscii();
    }

    {
        VtkSliceWriter<CartMesh> writer(cartMesh, domain, "slice-cell-int");
        writer.addScalarField(false, "function", Q.data());
        writer.writeAscii();
    }
}

TEST(VtkSliceWriter, NodeCenterData_double){

    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);
    Extent domain({.49, 0, 0}, {.49, 1, 1});

    std::vector<double> Q(cartMesh.numberOfNodes(), -1);
    Mesh<CartMesh> genMesh(cartMesh);

    for(int nodeId : range(cartMesh.numberOfNodes())){
        Point p;
        cartMesh.getNode(nodeId, &p[0]);
        Q[nodeId] = p[0] + p[1] + p[2];
    }

    {
        VtkWriter<CartMesh> writer(cartMesh, "full-node-double");
        writer.addScalarField(true, "function", Q.data());
        writer.writeAscii();
    }

    {
        VtkSliceWriter<CartMesh> writer(cartMesh, domain, "slice-node-double");
        writer.addScalarField(true, "function", Q.data());
        writer.writeAscii();
    }
}

TEST(VtkSliceWriter, NodeCenterData_float){

    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);
    Extent domain({.49, 0, 0}, {.49, 1, 1});

    std::vector<float> Q(cartMesh.numberOfNodes(), -1);
    Mesh<CartMesh> genMesh(cartMesh);

    for(int nodeId : range(cartMesh.numberOfNodes())){
        Point p;
        cartMesh.getNode(nodeId, &p[0]);
        Q[nodeId] = p[0] + p[1] + p[2];
    }

    {
        VtkWriter<CartMesh> writer(cartMesh, "full-node-float");
        writer.addScalarField(true, "function", Q.data());
        writer.writeAscii();
    }

    {
        VtkSliceWriter<CartMesh> writer(cartMesh, domain, "slice-node-float");
        writer.addScalarField(true, "function", Q.data());
        writer.writeAscii();
    }
}

TEST(VtkSliceWriter, NodeCenterData_int){
    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);

    Extent domain({.49, 0, 0}, {.49, 1, 1});

    std::vector<int> Q(cartMesh.numberOfNodes(), -1);
    Mesh<CartMesh> genMesh(cartMesh);

    for(int nodeId : range(cartMesh.numberOfNodes())){
        Q[nodeId] = nodeId;
    }

    {
        VtkWriter<CartMesh> writer(cartMesh, "full-node-int");
        writer.addScalarField(true, "function", Q.data());
        writer.writeAscii();
    }

    {
        VtkSliceWriter<CartMesh> writer(cartMesh, domain, "slice-node-int");
        writer.addScalarField(true, "function", Q.data());
        writer.writeAscii();
    }
}

TEST(VtkSliceWriter, JustWriteSliceMesh){

    CartMesh cartMesh({0, 0, 0}, {1, 1, 1}, 10, 10, 10);
    std::vector<double> myData(cartMesh.numberOfCells(), 0);
    myData[0] = 1.0;

    MeshSlice<CartMesh> meshSlice(cartMesh, false, myData.data(), 1, 0, 0.5, 1.5);
    VtkWriter<MeshSlice<CartMesh>> writer(meshSlice, "meshThreshold");
    writer.writeAscii();
}
