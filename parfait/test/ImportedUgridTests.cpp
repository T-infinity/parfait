#include <vector>
#include <catch.hpp>
#include <parfait/ImportedUgrid.h>
#include <parfait/GenericSurfaceMesh.h>

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