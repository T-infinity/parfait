#include "CppUTest/CommandLineTestRunner.h"
#include "parallel_mesh_importer.h"
#include "imported_ugrid.h"
#include "VtkGenericWriter.h"
#include "parmetis_prepper.h"
#include <vector>
#include <string>

using std::vector;
TEST_GROUP(ParallelMeshImporterTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

#if 0

TEST(ParallelMeshImporterTests,Exists)
{
	vector<std::string> grids;
	grids.push_back("grids/6cell.ugrid");
	grids.push_back("grids/6cell.ugrid");
	ParallelMeshImporter importer(grids);
	
	ImportedUgrid ugrid = importer.importUgrid();
	LONGS_EQUAL(28,ugrid.numberOfNodes());
	LONGS_EQUAL(12,ugrid.numberOfCells());


}
#endif

TEST(ParallelMeshImporterTests,readAndVisualize)
{
	using namespace MessagePasser;
	Init();
	vector<std::string> grids;
	vector<bool> isBigEndian;
	//grids.push_back("grids/sphere_in_sphere.ugrid");
	grids.push_back("grids/6cell.ugrid");
	isBigEndian.push_back(false);
	ParallelMeshImporter importer(grids,isBigEndian);
	
	ImportedUgrid ugrid = importer.importUgrid();
	ParMetisPrepper<ImportedUgrid> prepper(ugrid);
		
	prepper.buildNodeToNodeConnectivity();
	vector<int> part = prepper.getPartVector();

	for(int i=0;i<part.size();i++)
	{
		printf("Rank %i: part[%i] = %i\n",Rank(),i,part[i]);
	
	}
	
	
	
	
	// NOTE: do not try to make visualization, because the
	// mesh has not been redistributed according to the part
	// vector.  This means that the connectivity won't make any
	// sense yet (all in global Id space).
	//
	//VtkWriter<ImportedUgrid> writer(ugrid,"MESH");
	//writer.writeAscii();
	
	Finalize();
}
