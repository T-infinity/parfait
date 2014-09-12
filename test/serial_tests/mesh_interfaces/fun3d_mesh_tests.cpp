#include "fun3d_mesh.h"
#include "CppUTest/CommandLineTestRunner.h"
#include <vector>

using std::vector;
TEST_GROUP(Fun3DMeshTests)
{
	vector<double> x;
	vector<double> y;
	vector<double> z;
	vector<int> globalNodeIds;
	vector<int> imesh;
	vector<int> triangles;
	vector<int> quads;
	vector<int> tets;
	vector<int> pyramids;
	vector<int> prisms;
	vector<int> hexs;
	vector<int> triangleTags;
	vector<int> quadTags;
	void setup()
	{
		x = {0.0,1.0,1.0,0.0};
		y = {0.0,0.0,1.0,0.0};
		z = {0.0,0.0,0.0,1.0};

		triangles = {0,1,2,
					 0,1,3,
					 0,3,2,
					 2,3,1};
		tets = {0,1,2,3};
		globalNodeIds = {0,1,2,3};
		imesh = {0,0,0,0};
		triangleTags = {0,1,2,3};
	}
	void teardown()
	{
	}
};
TEST(Fun3DMeshTests,Exists)
{
	Fun3DMesh funGrid(x.size(),x.size(),
			x.data(),y.data(),z.data(),
			globalNodeIds.data(),
			imesh.data(),
			triangles.size()/3,triangles.data(),triangleTags.data(),
			triangleTags.data(),
			quads.size()/4,quads.data(),quadTags.data(),
			quadTags.data(),
			tets.size()/4,tets.data(),
			pyramids.size()/5,pyramids.data(),
			prisms.size(),prisms.data(),
			hexs.size(),hexs.data());

	LONGS_EQUAL(4,funGrid.numberOfNodes());
	LONGS_EQUAL(1,funGrid.numberOfCells());
	LONGS_EQUAL(4,funGrid.numberOfBoundaryFaces());
	LONGS_EQUAL(4,funGrid.numberOfFacesInCell(0));
	LONGS_EQUAL(3,funGrid.numberOfNodesInCellFace(0,0));

	for(int i=0;i<4;i++)
	{
		LONGS_EQUAL(globalNodeIds[i],funGrid.getGlobalNodeId(i));
		LONGS_EQUAL(0,funGrid.getImesh(i));
	}

	LONGS_EQUAL(3,funGrid.numberOfNodesInBoundaryFace(0));
}

template <class MeshType>
void constFunction(const MeshType &mesh)
{
    for(auto cell : mesh.cells()){
        for(auto face : cell){

        }
    }
}
TEST(Fun3DMeshTests, constCheck)
{
	Fun3DMesh funGrid(x.size(),x.size(),
			x.data(),y.data(),z.data(),
			globalNodeIds.data(),
			imesh.data(),
			triangles.size()/3,triangles.data(),triangleTags.data(),
			triangleTags.data(),
			quads.size()/4,quads.data(),quadTags.data(),
			quadTags.data(),
			tets.size()/4,tets.data(),
			pyramids.size()/5,pyramids.data(),
			prisms.size(),prisms.data(),
			hexs.size(),hexs.data());

    Mesh<Fun3DMesh> mesh(funGrid);
    constFunction(mesh);

}

