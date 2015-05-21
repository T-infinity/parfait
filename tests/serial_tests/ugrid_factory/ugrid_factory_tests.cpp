#include <string>
#include "imported_ugrid_factory.h"
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;

TEST_GROUP(ImportedUgridFactoryTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(ImportedUgridFactoryTests,Exists)
{
	std::string filename = SIX_CELL_TEST_GRID;
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid(filename);
	LONGS_EQUAL(18,ugrid.numberOfBoundaryFaces());
	for(int i=0;i<18;i++)
		LONGS_EQUAL(5050,ugrid.getBoundaryCondition(i));
}

