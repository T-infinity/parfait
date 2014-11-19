#include "vtk_surface_writer.h"
#include "imported_ugrid.h"
#include "message_passer.h"
#include <vector>
#include <string>
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;

TEST_GROUP(VtkSurfaceTests)
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

TEST(VtkSurfaceTests,Exists)
{
	using namespace MessagePasser;
	Init();
	// shift each tet so they don't overlap
	for(double &vertex:nodes)
		vertex += (double)Rank();
	// create grid on each proc
	ImportedUgrid ugrid(nodes,triangles,quads,
		tets,pyramids,prisms,hexs, triangleTags, quadTags,
		triangleBoundaryConditions,
		quadBoundaryConditions);
	// create writer on each proc	
	std::string meshName = "test";
	VtkSurfaceWriter<ImportedUgrid> writer(ugrid,meshName);

	vector<int> intField(4,9);
	writer.addScalarField(true,"Integer Field",intField.data());

	vector<float> floatField(4,1.1);
	writer.addScalarField(true,"Float Field",floatField.data());
	
	vector<double> doubleField(4,1.2);
	writer.addScalarField(true,"Double Field",doubleField.data());

	vector<int> cellIntField(20,3);
	writer.addVectorField(false,"Int vector field",5,cellIntField.data());

	vector<float> cellFloatField(20,3.3);
	writer.addVectorField(false,"Float vector field",5,cellFloatField.data());

	vector<double> cellDoubleField(20,3.6);
	writer.addVectorField(false,"Double vector field",5,cellDoubleField.data());

	writer.writeAscii();
	Finalize();	
}
