#include "CppUTest/CommandLineTestRunner.h"
#include "cell_to_cell.h"
#include "imported_ugrid_factory.h"
#include "range_loop.h"

TEST_GROUP(CellToCellTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

#if 0

TEST(CellToCellTests,Exists)
{
	ImportedUgrid ugrid = ImportedUgridFactory::readUgrid("grids/6cell.ugrid");

	vector<vector<int> > cell2cell = buildCellToCell(ugrid);
	for(int i:range(cell2cell))
	{
		printf("Cell %i connected to: ",i);
		for(int nbr:cell2cell[i])
			printf("%i ",nbr);
		printf("\n");
	}

    plotConnectivityBandwidth("6cell.connectivity.dat", cell2cell);

}
#endif
