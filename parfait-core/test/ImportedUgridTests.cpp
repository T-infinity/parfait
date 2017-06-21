#include "ImportedUgrid.h"
#include "RangeLoop.h"
#include <vector>
#include "GenericMesh.h"
#include "GenericSurfaceMesh.h"
#include <catch.hpp>

using std::vector;
using namespace Parfait;

template <class MeshType> void constFunction(const MeshType &mesh);

namespace ImportedUgridTests{
	vector<double> nodes;
	vector<int> triangles;
	vector<int> quads;
	vector<int> tets;
	vector<int> pyramids;
	vector<int> prisms;
	vector<int> hexs;
    vector<int> triangleTags;
    vector<int> quadTags;
    vector<int> triangleBoundaryConditions;
    vector<int> quadBoundaryConditions;
	void setup()
	{
		nodes = {0.0,0.0,0.0,
				 1.0,0.0,0.0,
				 1.0,1.0,0.0,
		         0.0,0.0,1.0};
		triangles = {0,1,2,
					 0,1,3,
					 0,3,2,
					 2,3,1};
        triangleTags = {1,2,3,4};
		tets = {0,1,2,3};
		triangleBoundaryConditions = {5050,5050,5050,5050};
	}
};

TEST_CASE("ImportedUgridTests,Exists") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);

	REQUIRE(4 == ugrid.numberOfNodes());
	REQUIRE(1 == ugrid.numberOfCells());
	REQUIRE(4 == ugrid.numberOfBoundaryFaces());
	REQUIRE(4 == ugrid.numberOfFacesInCell(0));
	REQUIRE(3 == ugrid.numberOfNodesInCellFace(0,0));
}

TEST_CASE("ImportedUgridTests,FaceTests") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	// create face object from first face of first cell in meshBuilder
	CellFace<ImportedUgrid> face(ugrid,0,0);
	REQUIRE(3 == face.numberOfNodes());

	auto faceNodes = face.getNodes();
}

TEST_CASE("ImportedUgridTests,CellTests") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	// create face object from first face of first cell in meshBuilder
	Cell<ImportedUgrid> cell(ugrid,0);
	REQUIRE(4 == cell.numberOfNodes());
	// get nodes and check that they are 0--3
	auto cellNodes = cell.getNodes();
	for(int i=0;i<4;i++)
		REQUIRE(i == cellNodes[i]);

	REQUIRE(4 == cell.numberOfFaces());

	Cell<ImportedUgrid>::Iterator cellIterator(cell,0);
}

TEST_CASE("ImportedUgridTests,CellCollectionTests") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	Mesh<ImportedUgrid> mesh(ugrid);	
	
	REQUIRE(4 == mesh.numberOfNodes());
	REQUIRE(1 == mesh.numberOfCells());
	REQUIRE(4 == mesh.numberOfFacesInCell(0));
	REQUIRE(3 == ugrid.numberOfNodesInCellFace(0,0));

	//printf("looping over cells in meshBuilder....\n");
    int cellId = 0;
	for(auto cell:mesh.cells())
	{
        REQUIRE(cellId++ == cell.Id());
		//printf("looping over faces in cell.....\n");
        int faceId = 0;
		for(auto face:cell)
		{
            REQUIRE(faceId++ == face.Id());
			//printf("looping over nodes in face...\n");
			for(int node:face)
			{
				double p[3];
				mesh.getNode(node,p);
				//printf("%lf %lf %lf\n",p[0],p[1],p[2]);
			}
		}
	}

	Node<ImportedUgrid> node(ugrid,0);
}

TEST_CASE("ImportedUgridTests,BoundaryFaceTests") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);

    SurfaceMesh<ImportedUgrid> mesh(ugrid);
    REQUIRE(4 == mesh.numberOfBoundaryFaces());
    for(auto id : range(mesh.numberOfBoundaryFaces())){
        REQUIRE(3 == mesh.numberOfNodesInBoundaryFace(id));
    }

    std::vector<int> nodes = mesh.getNodesInBoundaryFace(0);
    REQUIRE(0 == nodes[0]);
    REQUIRE(1 == nodes[1]);
    REQUIRE(2 == nodes[2]);

    nodes = mesh.getNodesInBoundaryFace(1);
    REQUIRE(0 == nodes[0]);
    REQUIRE(1 == nodes[1]);
    REQUIRE(3 == nodes[2]);

    nodes = mesh.getNodesInBoundaryFace(2);
    REQUIRE(0 == nodes[0]);
    REQUIRE(3 == nodes[1]);
    REQUIRE(2 == nodes[2]);

    nodes = mesh.getNodesInBoundaryFace(3);
    REQUIRE(2 == nodes[0]);
    REQUIRE(3 == nodes[1]);
    REQUIRE(1 == nodes[2]);
}

TEST_CASE("ImportedUgridTests, DirectCellAccess") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	Mesh<ImportedUgrid> mesh(ugrid);	

    auto cell = mesh.cell(0);
    REQUIRE(4 == cell.numberOfNodes());
}

TEST_CASE("ImportedUgridTests, DirectFaceAccess") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	Mesh<ImportedUgrid> mesh(ugrid);	
    auto cell = mesh.cell(0);
    auto face = cell.getFace(0);
    REQUIRE(3 == face.numberOfNodes());
}

template <class MeshType>
void constFunction(const MeshType &mesh) {
    //for(auto cell : meshBuilder.cells()){
    //    for(auto face : cell){

    //    }
    //}
}

TEST_CASE("ImportedUgridTests, constCheck") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
    Mesh<ImportedUgrid> mesh(ugrid);
    constFunction(mesh);

}
