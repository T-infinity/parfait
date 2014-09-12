#include "CppUTest/CommandLineTestRunner.h"
#include "imported_ugrid.h"
#include "message_passer.h"
#include "parallel_mesh_importer.h"
#include "vtk_generic_writer.h"
#include "vtk_surface_writer.h"
#include "parmetis_prepper.h"
#include "fun3d_mesh.h"
#include "parallel_mesh_redistributor.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
using namespace MessagePasser;

TEST_GROUP(ParallelMeshReDistributorTests)
{
	void setup()
	{
		Init();
	}
	void teardown()
	{
		Finalize();
	}
};

TEST(ParallelMeshReDistributorTests,Exists)
{
	Init();
	vector<string> grids;
	grids.push_back("grids/6cell.ugrid");
	grids.push_back("grids/sphere_in_sphere.ugrid");
	vector<bool> isBigEndian;
	isBigEndian.push_back(false);
	isBigEndian.push_back(false);

	ParallelMeshImporter importer(grids,isBigEndian);
	ImportedUgrid ugrid = importer.importUgrid();	

	ParMetisPrepper<ImportedUgrid> prepper(ugrid);
	prepper.buildNodeToNodeConnectivity();
	vector<int> part = prepper.getPartVector();
	//for(int &i:part)
	//	i = MessagePasser::Rank();
	vector<int> procNodeMap;
	AllGather(ugrid.numberOfNodes(),procNodeMap);
	procNodeMap.insert(procNodeMap.begin(),0);
	for(int i=1;i<procNodeMap.size();i++)
		procNodeMap[i] += procNodeMap[i-1];
	if(Rank() == 0)
	for(int i:range(procNodeMap))
		printf("%i ",procNodeMap[i]);
	printf("\n");


	ParallelMeshReDistributor distributor(ugrid,procNodeMap,part);

	Fun3DMesh mesh = distributor.createFun3DMesh();
	vector<int> partition(mesh.numberOfNodes(),Rank());

	VtkWriter<Fun3DMesh> writer(mesh,"herp");
	writer.addScalarField(true,"Partition Id",partition.data());
	writer.writeAscii();

	vector<int> boundaryTags(mesh.numberOfBoundaryFaces(),0);
	for(int i:range(boundaryTags))
		boundaryTags[i] = mesh.getBoundaryTag(i);
	printf("# boundary tags %i\n",boundaryTags.size());

	VtkSurfaceWriter<Fun3DMesh> surfaceWriter(mesh,"surface");
	surfaceWriter.addScalarField(false,"Boundary Tag",boundaryTags.data());
	surfaceWriter.writeAscii();	


	Finalize();

}
