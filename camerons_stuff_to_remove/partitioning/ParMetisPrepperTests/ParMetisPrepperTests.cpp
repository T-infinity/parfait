#include "CppUTest/CommandLineTestRunner.h"
#include "parmetis_prepper.h"
#include "imported_ugrid.h"
#include "message_passer.h"
#include <vector>

using std::vector;

TEST_GROUP(ParMetisPrepperTests)
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
		tets = {0,1,2,3};
	}
	void teardown()
	{
	}
};

TEST(ParMetisPrepperTests,Exists)
{

	for(int &id:tets)
		id += 4*Rank();

	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs,triangleTags,quadTags);

	ParMetisPrepper<ImportedUgrid> prepper(ugrid);
	prepper.buildNodeToNodeConnectivity();
	vector<int> part = prepper.getPartVector();
	// make sure part vector has a partition id for each node
	LONGS_EQUAL(ugrid.numberOfNodes(),(int)part.size());

	// since each proc has its own mesh of 1 tet,
	// the nodes should already be ideally distributed
	for(int partitionId:part)
		LONGS_EQUAL(Rank(),partitionId);

}

