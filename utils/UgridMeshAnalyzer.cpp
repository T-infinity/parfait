#include <GenericMesh.h>
#include <GenericMeshTools.h>
#include "UgridMeshAnalyzer.h"

UgridMeshAnalyzer::UgridMeshAnalyzer(
        Parfait::ImportedUgrid &ugrid_in)
        :ugrid(ugrid_in){

}
void UgridMeshAnalyzer::printStats() {
    printf("\nNumber of nodes:          %d", ugrid.numberOfNodes());
    printf("\nNumber of cells:          %d", ugrid.numberOfCells());
    printf("\nNumber of boundary faces: %d", ugrid.numberOfBoundaryFaces());

    printf("\n\nNumber of tets:           %d", ugrid.tets.size() / 4);
    printf("\nNumber of prisms:         %d", ugrid.prisms.size() / 6);
    printf("\nNumber of pyramids:       %d", ugrid.pyramids.size() / 5);
    printf("\nNumber of hexs:           %d", ugrid.hexs.size() / 8);
}
void UgridMeshAnalyzer::throwIfBadIndexFound() {
    for(int cellId = 0; cellId < ugrid.numberOfCells(); cellId++){
        auto nodes = ugrid.getNodesInCell(cellId);
        for(auto n : nodes)
            if(n < 0 or n > ugrid.numberOfNodes())
                throw std::logic_error("Found invalid id");
    }
}
void UgridMeshAnalyzer::throwIfNegativeVolumeFound() {
    Parfait::Mesh<Parfait::ImportedUgrid> mesh(ugrid);
    for(auto cell : mesh.cells()){
        double v = Parfait::GenericMeshTools::computeCellVolume(mesh, cell);
        if(v < 0)
            throw std::logic_error("Found a cell with negative volume");
    }
}
void UgridMeshAnalyzer::printNegativeVolumeStatistics() {
    std::vector<int> invalidElementCount = countNegativeVolumesByCellType();

    printf("\nNumber of invalid elements:\n");
    printf("\nTets      %d", invalidElementCount[Parfait::ImportedUgrid::TET]);
    printf("\nPyramids  %d", invalidElementCount[Parfait::ImportedUgrid::PYRAMID]);
    printf("\nPrisms    %d", invalidElementCount[Parfait::ImportedUgrid::PRISM]);
    printf("\nHexs      %d", invalidElementCount[Parfait::ImportedUgrid::HEX]);
}
std::vector<int> UgridMeshAnalyzer::countNegativeVolumesByCellType() {
    std::vector<int> invalidElementCount(4, 0);

    Parfait::Mesh<Parfait::ImportedUgrid> mesh(ugrid);
    for(int cellId = 0; cellId < ugrid.numberOfCells(); cellId++){
        auto cell = mesh.cell(cellId);
        double v = Parfait::GenericMeshTools::computeCellVolume(mesh, cell);
        if(v < 0){
            invalidElementCount[ugrid.getCellType(cellId)]++;
        }
    }
    return invalidElementCount;
}
