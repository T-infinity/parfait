#include "ImportedUgrid.h"
#include "RangeLoop.h"
#include <vector>
#include "GenericMesh.h"
#include "GenericSurfaceMesh.h"
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;
using namespace Parfait;

template <class MeshType> void constFunction(const MeshType &mesh);

TEST_GROUP(ImportedUgridTests)
{
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
	void teardown()
	{
	}
};

TEST(ImportedUgridTests,Exists)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);

	LONGS_EQUAL(4,ugrid.numberOfNodes());
	LONGS_EQUAL(1,ugrid.numberOfCells());
	LONGS_EQUAL(4,ugrid.numberOfBoundaryFaces());
	LONGS_EQUAL(4,ugrid.numberOfFacesInCell(0));
	LONGS_EQUAL(3,ugrid.numberOfNodesInCellFace(0,0));
	
}

TEST(ImportedUgridTests,FaceTests)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	// create face object from first face of first cell in mesh	
	CellFace<ImportedUgrid> face(ugrid,0,0);
	LONGS_EQUAL(3,face.numberOfNodes());

	auto faceNodes = face.getNodes();

	CellFace<ImportedUgrid>::Iterator faceIterator(face,0);
	CellFace<ImportedUgrid>::Iterator beginIterator = face.begin();
	CellFace<ImportedUgrid>::Iterator endIterator = face.end();
}

TEST(ImportedUgridTests,CellTests)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	// create face object from first face of first cell in mesh	
	Cell<ImportedUgrid> cell(ugrid,0);
	LONGS_EQUAL(4,cell.numberOfNodes());
	// get nodes and check that they are 0--3
	auto cellNodes = cell.getNodes();
	for(int i=0;i<4;i++)
		LONGS_EQUAL(i,cellNodes[i]);

	LONGS_EQUAL(4,cell.numberOfFaces());

	Cell<ImportedUgrid>::Iterator cellIterator(cell,0);

#if 0	
	for(auto face:cell)
	{
		printf("Looping over face....\n");
		for(auto node:face)
		{
			double p[3];
			ugrid.getNode(node,p);
			printf("%lf %lf %lf\n",p[0],p[1],p[2]);
		}
	}
#endif
}

TEST(ImportedUgridTests,CellCollectionTests)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	Mesh<ImportedUgrid> mesh(ugrid);	
	
	LONGS_EQUAL(4,mesh.numberOfNodes());
	LONGS_EQUAL(1,mesh.numberOfCells());
	LONGS_EQUAL(4,mesh.numberOfFacesInCell(0));
	LONGS_EQUAL(3,ugrid.numberOfNodesInCellFace(0,0));

	//printf("looping over cells in mesh....\n");
    int cellId = 0;
	for(auto cell:mesh.cells())
	{
        LONGS_EQUAL(cellId++, cell.Id());
		//printf("looping over faces in cell.....\n");
        int faceId = 0;
		for(auto face:cell)
		{
            LONGS_EQUAL(faceId++, face.Id());
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

TEST(ImportedUgridTests,BoundaryFaceTests)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);

    SurfaceMesh<ImportedUgrid> mesh(ugrid);
    LONGS_EQUAL(4, mesh.numberOfBoundaryFaces());
    for(auto id : range(mesh.numberOfBoundaryFaces())){
        LONGS_EQUAL(3, mesh.numberOfNodesInBoundaryFace(id));
    }

    std::vector<int> nodes = mesh.getNodesInBoundaryFace(0);
    LONGS_EQUAL(0, nodes[0]);
    LONGS_EQUAL(1, nodes[1]);
    LONGS_EQUAL(2, nodes[2]);

    nodes = mesh.getNodesInBoundaryFace(1);
    LONGS_EQUAL(0, nodes[0]);
    LONGS_EQUAL(1, nodes[1]);
    LONGS_EQUAL(3, nodes[2]);

    nodes = mesh.getNodesInBoundaryFace(2);
    LONGS_EQUAL(0, nodes[0]);
    LONGS_EQUAL(3, nodes[1]);
    LONGS_EQUAL(2, nodes[2]);

    nodes = mesh.getNodesInBoundaryFace(3);
    LONGS_EQUAL(2, nodes[0]);
    LONGS_EQUAL(3, nodes[1]);
    LONGS_EQUAL(1, nodes[2]);
}

TEST(ImportedUgridTests, DirectCellAccess)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	Mesh<ImportedUgrid> mesh(ugrid);	

    auto cell = mesh.cell(0);
    LONGS_EQUAL(4, cell.numberOfNodes());
}

TEST(ImportedUgridTests, DirectFaceAccess)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
	Mesh<ImportedUgrid> mesh(ugrid);	
    auto cell = mesh.cell(0);
    auto face = cell.getFace(0);
    LONGS_EQUAL(3, face.numberOfNodes());
}

template <class MeshType>
void constFunction(const MeshType &mesh)
{
    for(auto cell : mesh.cells()){
        for(auto face : cell){

        }
    }
}
TEST(ImportedUgridTests, constCheck)
{
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);
    Mesh<ImportedUgrid> mesh(ugrid);
    constFunction(mesh);

}
