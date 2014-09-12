#include "CppUTest/CommandLineTestRunner.h"
#include "extent_builder.h"
#include <vector>

using std::vector;

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

	Extent extent = ExtentBuilder::build(mesh);
	DOUBLES_EQUAL(0.0,extent.lo[0],tol);
	DOUBLES_EQUAL(0.0,extent.lo[1],tol);
	DOUBLES_EQUAL(0.0,extent.lo[2],tol);
	DOUBLES_EQUAL(1.0,extent.hi[0],tol);
	DOUBLES_EQUAL(1.0,extent.hi[1],tol);
	DOUBLES_EQUAL(1.0,extent.hi[2],tol);
}
