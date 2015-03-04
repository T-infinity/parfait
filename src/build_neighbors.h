#define once
#include "generic_mesh.h"
#include "node_to_cell.h"
#include <vector>

template <class MeshType>
class NeighborBuilder{
private:
    Mesh<MeshType> mesh;
    std::vector<std::vector<int>> nodeToCell;

public:
    std::vector<std::vector<int>> neighbors;

    NeighborBuilder(MeshType &mesh_interface)
    : mesh(mesh_interface), neighbors(mesh.numberOfCells()){
        for(int cellId = 0; cellId < mesh.numberOfCells(); cellId++){
            neighbors[cellId].resize(mesh.numberOfFacesInCell(cellId), -1);
        }

        nodeToCell = buildNodeToCell(mesh_interface);

        for(auto cell : mesh.cells()){
            auto cellNodes = cell.getNodes();
            std::vector<int> neighborMaybe;
            for(int cellNode : cellNodes){
                for(int candidate : nodeToCell[cellNode])
                    insertUnique(neighborMaybe, candidate);
            }
            for(int candidateId : neighborMaybe){
                if(candidateId == cell.Id()) continue;
                for(auto face: cell){
                    for(auto neighborFace : mesh.cell(candidateId)){
                        if(facesMatch(face, neighborFace)){
                            neighbors[cell.Id()][face.Id()] = candidateId;
                            break;
                        }
                    }
                }
            }
        }
    }
};

template <class MeshType>
std::vector<std::vector<int>> buildNeighbors(MeshType &mesh_interface){

    NeighborBuilder<MeshType> neighborBuilder(mesh_interface);
    return std::move(neighborBuilder.neighbors);
}