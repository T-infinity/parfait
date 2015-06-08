#include "cart_block.h"
#include "point.h"
#include <cmath>
#include "CppUTest/CommandLineTestRunner.h"
using namespace Parfait;

TEST_GROUP(CartBlockTests)
{
    double tol;
	void setup()
	{
        tol = 1.0e-14;
	}
	void teardown()
	{
	}
};

TEST(CartBlockTests,Exists)
{
    CartBlock block;
}

TEST(CartBlockTests, DimensionCheck)
{

    double p1[3] = {1.0,2.0,3.0};
    double p2[3] = {2.2,3.2,4.2};

	CartBlock block(p1,p2);
    double lo[3];
    double hi[3];
    //block.get_lo_point(lo);
    //block.get_hi_point(hi);
    lo[0] = block.lo[0];
    lo[1] = block.lo[1];
    lo[2] = block.lo[2];

    hi[0] = block.hi[0];
    hi[1] = block.hi[1];
    hi[2] = block.hi[2];

    CHECK_EQUAL(p1[0], lo[0]);
    CHECK_EQUAL(p1[1], lo[1]);
    CHECK_EQUAL(p1[2], lo[2]);

    CHECK_EQUAL(p2[0], hi[0]);
    CHECK_EQUAL(p2[1], hi[1]);
    CHECK_EQUAL(p2[2], hi[2]);

}

TEST(CartBlockTests, Cells)
{
    double p1[3] = {0.0,0.0,0.0};
    double p2[3] = {1.0,1.0,1.0};

    int numCells_x = 10;
    int numCells_y = 5;
    int numCells_z = 10;
    CartBlock block(p1, p2, numCells_x, numCells_y, numCells_z);
    
    DOUBLES_EQUAL(0.1, block.get_dx(), tol);
    DOUBLES_EQUAL(0.2, block.get_dy(), tol);
    DOUBLES_EQUAL(0.1, block.get_dz(), tol);

    CHECK_EQUAL(10, block.numberOfCells_X());
    CHECK_EQUAL( 5, block.numberOfCells_Y());
    CHECK_EQUAL(10, block.numberOfCells_Z());

    int numberOfCells = 500;
    CHECK_EQUAL(numberOfCells, block.numberOfCells());

    int numberOfNodes = 726;
    CHECK_EQUAL(numberOfNodes, block.numberOfNodes());
}

TEST(CartBlockTests, NodeRetrieval)
{
    double p1[3] = {1.0,2.0,3.0};
    double p2[3] = {2.2,3.2,4.2};

    int numCells_x = 5;
    int numCells_y = 6;
    int numCells_z = 7;
    int numNodes_x = 6;
    int numNodes_y = 7;
    int numNodes_z = 8;
    CartBlock block(p1, p2, numCells_x, numCells_y, numCells_z);
    
    double p4[3], p5[3];

    int numNodes = numNodes_x*numNodes_y*numNodes_z;
	//block.get_lo_point(p4);
    p4[0] = block.lo[0];
    p4[1] = block.lo[1];
    p4[2] = block.lo[2];
	block.getNode(0,p5);

    DOUBLES_EQUAL(1.0, p4[0], tol);
    DOUBLES_EQUAL(2.0, p4[1], tol);
    DOUBLES_EQUAL(3.0, p4[2], tol);

    DOUBLES_EQUAL(1.0, p5[0], tol);
    DOUBLES_EQUAL(2.0, p5[1], tol);
    DOUBLES_EQUAL(3.0, p5[2], tol);

	//block.get_hi_point(p4);
    p4[0] = block.hi[0];
    p4[1] = block.hi[1];
    p4[2] = block.hi[2];
	block.getNode(numNodes-1,p5);

    DOUBLES_EQUAL(2.2, p4[0], tol);
    DOUBLES_EQUAL(3.2, p4[1], tol);
    DOUBLES_EQUAL(4.2, p4[2], tol);

    DOUBLES_EQUAL(2.2, p5[0], tol);
    DOUBLES_EQUAL(3.2, p5[1], tol);
    DOUBLES_EQUAL(4.2, p5[2], tol);

	//block.get_hi_point(p4);
    p4[0] = block.hi[0];
    p4[1] = block.hi[1];
    p4[2] = block.hi[2];

	p4[2] -= block.getLength_Z();
	block.getNode(numCells_x+numNodes_x*(numCells_y),p5);

    DOUBLES_EQUAL(2.2, p4[0], tol);
    DOUBLES_EQUAL(3.2, p4[1], tol);
    DOUBLES_EQUAL(3.0, p4[2], tol);
}

TEST(CartBlockTests, LegacyTests){

	double p1[3],p2[3],p3[3],extent[6];
	p1[0] = 1.0;
	p1[1] = 2.0;
	p1[2] = 3.0;
	p2[0] = 2.2;
	p2[1] = 3.2;
	p2[2] = 4.2;
	p3[0] = 1.7;
	p3[1] = 2.7;
	p3[2] = 3.7;

	extent[0] = p1[0];
	extent[1] = p1[1];
	extent[2] = p1[2];
	extent[3] = p2[0];
	extent[4] = p2[1];
	extent[5] = p2[2];

	int ncells_x,ncells_y,ncells_z;
	int nnodes_x,nnodes_y,nnodes_z;
	ncells_x = 5;
	ncells_y = 6;
	ncells_z = 7;
	nnodes_x = ncells_x+1;
	nnodes_y = ncells_y+1;
	nnodes_z = ncells_z+1;
	CartBlock a(p1,p2);
	CartBlock b(extent);
	CartBlock c(p1,p2,ncells_x,ncells_y,ncells_z);
	CartBlock d(extent,ncells_x,ncells_y,ncells_z);

	a.setDimensions(ncells_x,ncells_y,ncells_z);
	b.setDimensions(ncells_x,ncells_y,ncells_z);

	double tol = 1.0e-15;
	double dx[4],dy[4],dz[4];
	dx[0] = a.get_dx();
	dx[1] = b.get_dx();
	dx[2] = c.get_dx();
	assert(fabs(dx[0] - dx[1]) < tol);
	assert(fabs(dx[0] - dx[2]) < tol);	
	dy[0] = a.get_dy();
	dy[1] = b.get_dy();
	dy[2] = c.get_dy();
	assert(fabs(dy[0] - dy[1]) < tol);
	assert(fabs(dy[0] - dy[2]) < tol);	
	dz[0] = a.get_dz();
	dz[1] = b.get_dz();
	dz[2] = c.get_dz();
	assert(fabs(dz[0] - dz[1]) < tol);
	assert(fabs(dz[0] - dz[2]) < tol);

	double my_dx,my_dy,my_dz;
	my_dx = (p2[0] - p1[0])/(double)ncells_x;
	my_dy = (p2[1] - p1[1])/(double)ncells_y;
	my_dz = (p2[2] - p1[2])/(double)ncells_z;
	assert(fabs(my_dx-dx[0]) < tol);
	assert(fabs(my_dy-dy[0]) < tol);
	assert(fabs(my_dz-dz[0]) < tol);

	int my_ncells = ncells_x*ncells_y*ncells_z;
    CHECK_EQUAL(my_ncells, a.numberOfCells());
	int my_nnodes = (ncells_x+1)*(ncells_y+1)*(ncells_z+1);
    CHECK_EQUAL(my_nnodes, a.numberOfNodes());

	// this also relies on the centroid being in the right cell
	int cell_id = a.getIdOfContainingCell(p3);
	double lo[3],hi[3];
	Extent cell = a.createExtentFromCell(cell_id);
	//cell.get_lo_point(lo);
	//cell.get_hi_point(hi);
	assert(cell.contains(Point(p3)));

	std::vector<int> ids;
	// make a box around the whole mesh, and make sure you get
	// all the cells
	Extent query_box(p1,p2);
	ids = a.getCellIdsInExtent(query_box);
	assert(my_ncells == (int)ids.size());
	
	// make a box that does not overlap the mesh and assert that it returns no cells
	extent[0] = -10.0;
	extent[1] = -10.0;
	extent[2] = -10.0;
	extent[3] = 0.0;
	extent[4] = 10.0;
	extent[5] = 10.0;
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);

	assert((int)ids.size() == 0);

	// make a box bigger than the mesh and make sure it returns all of them
	extent[3] = 10.0;
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(my_ncells == (int)ids.size());

	// make a box that only touches the -z face
	extent[3] = 10.0;
	extent[5] = p1[2];
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(ncells_x*ncells_y == (int)ids.size());

	// make a box that only touches the +z face
	extent[5] = 10.0;
	extent[2] = p2[2];
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(ncells_x*ncells_y == (int)ids.size());
	
	// make a box that only touches the -x face
	extent[2] = -10.0;
	extent[3] = p1[0];
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(ncells_z*ncells_y == (int)ids.size());

	// make a box that only touches the +x face
	extent[3] = 10.0;
	extent[0] = p2[0];
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(ncells_z*ncells_y == (int)ids.size());
	
	// make a box that only touches the -y face
	extent[0] = -10.0;
	extent[4] = p1[1];
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(ncells_z*ncells_x == (int)ids.size());
	
	// make a box that only touches the +y face
	extent[4] = 10.0;
	extent[1] = p2[1];
	//query_box.resize(extent);
    query_box = Extent(extent);
	ids = a.getCellIdsInExtent(query_box);
	assert(ncells_z*ncells_x == (int)ids.size());
		
	double p4[3],p5[3];
	// check that node 0 returns the coords of the lo point
	//a.get_lo_point(p4);
    p4[0] = a.lo[0];
    p4[1] = a.lo[1];
    p4[2] = a.lo[2];
	a.getNode(0,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);
	// same check for hi point
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];
	a.getNode(my_nnodes-1,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);

	// check other corners
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];
	p4[2] -= a.getLength_Z();
	a.getNode(ncells_x+nnodes_x*(ncells_y),p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);
	// check other corners
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];

    p4[0] = a.hi[0];
	p4[2] -= a.getLength_Z();
	p4[1] -= a.getLength_Y();
	a.getNode(ncells_x,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);
	// check other corners
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];
	p4[1] -= a.getLength_Y();
	a.getNode(ncells_x+nnodes_x*nnodes_y*ncells_z,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);
	// check other corners
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];
	p4[0] -= a.getLength_X();
	a.getNode(nnodes_x*ncells_y+nnodes_x*nnodes_y*ncells_z,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);
	// check other corners
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];
	p4[0] -= a.getLength_X();
	p4[1] -= a.getLength_Y();
	a.getNode(nnodes_x*nnodes_y*ncells_z,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);
	// check other corners
	//a.get_hi_point(p4);
    p4[0] = a.hi[0];
    p4[1] = a.hi[1];
    p4[2] = a.hi[2];
	p4[0] -= a.getLength_X();
	p4[2] -= a.getLength_Z();
	a.getNode(nnodes_x*ncells_y,p5);
	assert(fabs(p5[0]-p4[0]) < 1.0e-15);
	assert(fabs(p5[1]-p4[1]) < 1.0e-15);
	assert(fabs(p5[2]-p4[2]) < 1.0e-15);

}



















