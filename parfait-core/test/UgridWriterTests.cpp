#include <vector>
#include <UgridReader.h>
#include <RangeLoop.h>
#include <UgridWriter.h>
#include <ImportedUgridFactory.h>
#include <catch.hpp>

using std::vector;
using namespace Parfait;
//TODO: add grid to repo
#if 0
namespace ugridWriterTests{
    std::string gridFile;
    int nnodes,ntri,nquad,ntet,npyr,nprism,nhex;
    vector<double> nodes;
    vector<int> triangles;
    vector<int> quads;
    vector<int> tets;
    vector<int> pyramids;
    vector<int> prisms;
    vector<int> hexs;
    vector<int> triTags;
    vector<int> quadTags;

    void setup() {
        using namespace UgridReader;
        gridFile = SIX_CELL_TEST_CASE_GRID;
        readHeader(gridFile,nnodes,ntri,nquad,ntet,npyr,nprism,nhex);
        nodes = readNodes(gridFile);
        triangles = readTriangles(gridFile);
        quads = readQuads(gridFile);
        tets = readTets(gridFile);
        pyramids = readPyramids(gridFile);
        prisms = readPrisms(gridFile);
        hexs  = readHexs(gridFile);
        triTags = readTriangleBoundaryTags(gridFile);
        quadTags = readQuadBoundaryTags(gridFile);
	}

    bool isTestGridValid(std::string newGridName){
        using namespace UgridReader;
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

        REQUIRE(nnodes   == nnodes2);
        REQUIRE(ntri     == ntri2);
        REQUIRE(npyr     == npyr2);
        REQUIRE(nprism   == nprism2);
        REQUIRE(ntet     == ntet2);
        REQUIRE(nhex     == nhex2);

        REQUIRE(nnodes == nodes.size()/3);
        REQUIRE(ntri   == triangles.size()/3);
        REQUIRE(nquad  == quads.size()/4);
        REQUIRE(ntet   == tets.size()/4);
        REQUIRE(npyr   == pyramids.size()/5);
        REQUIRE(nprism == prisms.size()/6);
        REQUIRE(nhex   == hexs.size()/8);
        REQUIRE(ntri   == triTags.size());
        REQUIRE(nquad  == quadTags.size());

        for(int i:range(nodes))
            REQUIRE(nodes[i] == nodes2[i]);
        for(int i:range(triangles))
            REQUIRE(triangles[i] == triangles2[i]);
        for(int i:range(quads))
            REQUIRE(quads[i] == quads2[i]);
        for(int i:range(tets))
            REQUIRE(tets[i] == tets2[i]);
        for(int i:range(pyramids))
            REQUIRE(pyramids[i] == pyramids2[i]);
        for(int i:range(prisms))
            REQUIRE(prisms[i] == prisms2[i]);
        for(int i:range(hexs))
            REQUIRE(hexs[i] == hexs2[i]);
        for(int i:range(triTags))
            REQUIRE(triTags[i] == triTags2[i]);
        for(int i:range(quadTags))
            REQUIRE(quadTags[i] == quadTags2[i]);

		return true;
    }
}

using  namespace ugridWriterTests;

TEST_CASE("ugridWriterTests, Class"){
    setup();
    UgridWriterFactory ugridWriter;
    ugridWriter.setName("test1");
    ugridWriter.setNodes(nodes.data(), nnodes);
    ugridWriter.setTets(tets.data(), ntet);
    ugridWriter.setHexes(hexs.data(), nhex);
    ugridWriter.setPyramids(pyramids.data(), npyr);
    ugridWriter.setPrisms(prisms.data(), nprism);
    ugridWriter.setTriangles(triangles.data(), ntri);
    ugridWriter.setQuads(quads.data(), nquad);
    ugridWriter.setTriangleTags(triTags.data());
    ugridWriter.setQuadTags(quadTags.data());

    ugridWriter.writeFile();
    CHECK(isTestGridValid("test1.ugrid"));
}

TEST_CASE("ugridWriterTests,Exists")
{
    setup();
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

    CHECK(isTestGridValid(newGridName));
}

TEST_CASE("ugridWriterTests,WriteWithOneFunctionCall")
{
    setup();
	std::string gridFile = SIX_CELL_TEST_CASE_GRID;
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

	auto ugrid = ImportedUgridFactory::readUgrid(gridFile,false);

	std::string newGridName = "test.ugrid";
	using namespace UgridWriter;
	writeImportedUgrid(ugrid,newGridName,false);

  REQUIRE(isTestGridValid(newGridName));

}
#endif
