#include "CppUTest/CommandLineTestRunner.h"
#include "parfait.h"
#include <vector>

using std::vector;

TEST_GROUP(ugridWriterTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(ugridWriterTests,Exists)
{
	std::string gridFile = "../grids/6cell.ugrid";
	using namespace UgridReader;
	int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
	readHeader(gridFile,nnodes,ntri,nquad,ntet,npyr,nprism,nhex);
	vector<double> nodes = readNodes(gridFile);
	vector<int> triangles = readTriangles(gridFile);
	vector<int> quads = readQuads(gridFile);
	vector<int> tets = readTets(gridFile);
	vector<int> pyramids = readPyramids(gridFile);
	vector<int> prisms = readPrisms(gridFile);
	vector<int> hexs  = readHexs(gridFile);
	vector<int> triTags = readTriangleBoundaryTags(gridFile);
	vector<int> quadTags = readQuadBoundaryTags(gridFile);

	std::string newGridName = "test.ugrid";
	using namespace UgridWriter;
	writeHeader(newGridName,nnodes,ntri,nquad,ntet,npyr,nprism,nhex,false);
	writeNodes(newGridName,nnodes,nodes.data(),false);
	writeTriangles(newGridName,ntri,triangles.data(),false);
	writeQuads(newGridName,nquad,quads.data(),false);
	writeTriangleBoundaryTags(newGridName,ntri,triTags.data(),false);
	writeQuadBoundaryTags(newGridName,nquad,quadTags.data(),false);
	writeTets(newGridName,ntet,tets.data(),false);
	writePyramids(newGridName,npyr,pyramids.data(),false);
	writePrisms(newGridName,nprism,prisms.data(),false);
	writeHexs(newGridName,nhex,hexs.data(),false);


	int nnodes2,ntri2,nquad2,ntet2,npyr2,nprism2,nhex2;
	readHeader(newGridName,nnodes2,ntri2,nquad2,ntet2,npyr2,nprism2,nhex2);
	vector<double> nodes2 = readNodes(newGridName);
	vector<int> triangles2 = readTriangles(newGridName);
	vector<int> quads2 = readQuads(newGridName);
	vector<int> tets2 = readTets(newGridName);
	vector<int> pyramids2 = readPyramids(newGridName);
	vector<int> prisms2 = readPrisms(newGridName);
	vector<int> hexs2  = readHexs(newGridName);
	vector<int> triTags2 = readTriangleBoundaryTags(newGridName);
	vector<int> quadTags2 = readQuadBoundaryTags(newGridName);

	assert(nnodes   == nnodes2);
	assert(ntri     == ntri2);
	assert(npyr     == npyr2);
	assert(nprism   == nprism2);
	assert(ntet     == ntet2);
	assert(nhex     == nhex2);

	assert(nnodes == nodes.size()/3);
	assert(ntri   == triangles.size()/3);
	assert(nquad  == quads.size()/4);
	assert(ntet   == tets.size()/4);
	assert(npyr   == pyramids.size()/5);
	assert(nprism == prisms.size()/6);
	assert(nhex   == hexs.size()/8);
	assert(ntri   == triTags.size());
	assert(nquad  == quadTags.size());

	for(int i:range(nodes))
		assert(nodes[i] == nodes2[i]);
	for(int i:range(triangles))
		assert(triangles[i] == triangles2[i]);
	for(int i:range(quads))
		assert(quads[i] == quads2[i]);
	for(int i:range(tets))
		assert(tets[i] == tets2[i]);
	for(int i:range(pyramids))
		assert(pyramids[i] == pyramids2[i]);
	for(int i:range(prisms))
		assert(prisms[i] == prisms2[i]);
	for(int i:range(hexs))
		assert(hexs[i] == hexs2[i]);
	for(int i:range(triTags))
		assert(triTags[i] == triTags2[i]);
	for(int i:range(quadTags))
		assert(quadTags[i] == quadTags2[i]);
}
