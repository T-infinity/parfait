#include <string>
#include "ImportedUgridFactory.h"
#include <catch.hpp>

using std::vector;
using namespace Parfait;

TEST_CASE("ImportedUgridFactoryTests,Exists")
{
	// TODO: add grid to repo
#if 0
	std::string filename = SIX_CELL_TEST_GRID;
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid(filename);
	REQUIRE(18 == ugrid.numberOfBoundaryFaces());
#endif
}

