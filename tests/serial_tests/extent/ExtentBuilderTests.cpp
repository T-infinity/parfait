#include "ExtentBuilder.h"
#include <vector>
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;
using namespace Parfait;
TEST_GROUP(ExtentBuilderTests)
{
	class PointCloud
	{
		public:
			int numberOfNodes() const {return nodes.size()/3;}
			void getNode(int id,double coords[3]) const
			{
				coords[0] = nodes[3*id+0];
				coords[1] = nodes[3*id+1];
				coords[2] = nodes[3*id+2];
			}
			vector<double> nodes;
	};

	PointCloud mesh;
	double tol;
	void setup()
	{
		tol = 1.0e-15;
	}
	void teardown()
	{
	}
};

TEST(ExtentBuilderTests,Exists)
{
	mesh.nodes = {0.0, 0.0, 0.0,
				  1.0, 0.0, 0.0,
				  0.5, 1.0, 0.3,
				  0.7, 0.1, 1.0};

	Extent<double> extent = ExtentBuilder::build(mesh);
	DOUBLES_EQUAL(0.0,extent.lo[0],tol);
	DOUBLES_EQUAL(0.0,extent.lo[1],tol);
	DOUBLES_EQUAL(0.0,extent.lo[2],tol);
	DOUBLES_EQUAL(1.0,extent.hi[0],tol);
	DOUBLES_EQUAL(1.0,extent.hi[1],tol);
	DOUBLES_EQUAL(1.0,extent.hi[2],tol);
}

TEST(ExtentBuilderTests,addPointToExtent)
{
	std::vector<Point<double>> points;
	points.push_back({0.0, 0.0, 0.});
	points.push_back({1.0, 0.0, 0.0});
	points.push_back({0.5, 1.0, 0.3});
	points.push_back({0.7, 0.1, 1.0});
	// initialize zero volume box at the origin
	Extent<double> e({.5,.5,.5},{.5,.5,.5});
	for(int i=0;i<4;i++)
		ExtentBuilder::addPointToExtent(e,points[i]);
	DOUBLES_EQUAL(0.0,e.lo[0],tol);
	DOUBLES_EQUAL(0.0,e.lo[1],tol);
	DOUBLES_EQUAL(0.0,e.lo[2],tol);
	DOUBLES_EQUAL(1.0,e.hi[0],tol);
	DOUBLES_EQUAL(1.0,e.hi[1],tol);
	DOUBLES_EQUAL(1.0,e.hi[2],tol);
};
