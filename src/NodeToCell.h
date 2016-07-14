#pragma once

#include <vector>
#include "VectorTools.h"
#include <GenericMesh.h>

namespace Parfait {
    template<class MeshType>
    std::vector<std::vector<int>> buildNodeToCell(MeshType &mesh_in) {
        Mesh<MeshType> mesh(mesh_in);

        std::vector<std::vector<int>> nodeToCell(mesh.numberOfNodes());

        for (auto cell : mesh.cells()) {
            auto cellNodes = cell.getNodes();
            for (int node : cellNodes) {
                insertUnique(nodeToCell[node], cell.Id());
            }
        }
        return nodeToCell;
    }
}