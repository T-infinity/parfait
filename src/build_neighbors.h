#define once
#include "generic_mesh.h"
#include "node_to_cell.h"
#include <vector>
#include <tuple>

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
            setCellNeighbors(cell);
        }
    }

    void setCellNeighbors(Cell<MeshType> &cell) {
        auto cellNodes = cell.getNodes();
        for(auto face: cell){
            std::vector<int> neighborMaybe = nodeToCell[face.getNodes()[0]];
            if(neighbors[cell.Id()][face.Id()] != -1) continue;
            int neighborCellId, neighborFaceId;
            std::tie(neighborCellId, neighborFaceId) = getCellIdAndFaceIdOfNeighbor(cell, neighborMaybe, face);
            neighbors[cell.Id()][face.Id()] = neighborCellId;
            if(neighborCellId != -1)
                neighbors[neighborCellId][neighborFaceId] = cell.Id();
        }
    }

    std::tuple<int, int> getCellIdAndFaceIdOfNeighbor(Cell<MeshType> &cell, vector<int> &neighborMaybe, CellFace<MeshType> &face) {
        for(int candidateId : neighborMaybe){
            if(candidateId == cell.Id()) continue;
            int faceId = 0;
            auto faceNodes = face.getNodes();
            std::sort(faceNodes.begin(), faceNodes.end());
            for(auto neighborFace : mesh.cell(candidateId)){
                auto neighborNodes = neighborFace.getNodes();
                std::sort(neighborNodes.begin(), neighborNodes.end());
                if(faceNodes == neighborNodes)
                    return std::tuple<int,int>{candidateId, faceId};
                faceId++;
            }
        }
        return std::tuple<int,int>{-1,-1};
    }

    std::vector<int> getCandidateNeighbors(vector<int> &cellNodes) {
        vector<int> neighborMaybe;
        for(int cellNode : cellNodes){
            for(int candidate : nodeToCell[cellNode])
                insertUnique(neighborMaybe, candidate);
        }
        return neighborMaybe;
    }
};

template <class MeshType>
std::vector<std::vector<int>> buildNeighbors(MeshType &mesh_interface){

    NeighborBuilder<MeshType> neighborBuilder(mesh_interface);
    return std::move(neighborBuilder.neighbors);
}