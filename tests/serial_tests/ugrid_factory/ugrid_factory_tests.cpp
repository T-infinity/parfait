#include <string>
#include "ImportedUgridFactory.h"
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;
using namespace Parfait;

TEST_GROUP(ImportedUgridFactoryTests)
{

};

TEST(ImportedUgridFactoryTests,Exists)
{
	std::string filename = SIX_CELL_TEST_GRID;
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid(filename);
	LONGS_EQUAL(18,ugrid.numberOfBoundaryFaces());
}

