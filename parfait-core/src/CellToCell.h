#pragma once

#include <stdio.h>
#include <algorithm>
#include <assert.h>
#include <algorithm>
#include <array>

#include "VectorTools.h"
#include "GenericMeshTools.h"
#include "Adt3dExtent.h"
#include "AdtBuilder.h"
#include "ExtentBuilder.h"
#include "NodeToCell.h"

#include <string>

namespace Parfait {
    template<typename T>
    bool facesMatch(T faceOne, T faceTwo) {
        auto face1Nodes = faceOne.getNodes();
        auto face2Nodes = faceTwo.getNodes();
        if (face1Nodes.size() != face2Nodes.size())
            return false;
        std::sort(face1Nodes.begin(), face1Nodes.end());
        std::sort(face2Nodes.begin(), face2Nodes.end());

        return face1Nodes == face2Nodes;
    }

    inline std::vector<int> getSharedCells(int node1, int node2,
                                           std::vector<std::vector<int>> &nodeToCell) {

        auto &cells1 = nodeToCell[node1];
        auto &cells2 = nodeToCell[node2];

        std::vector<int> shared;
        for (int cell : cells1) {
            if (isIn(cells2, cell)) {
                shared.push_back(cell);
            }
        }
        return shared;
    }

    template<typename MeshType>
    std::vector<std::vector<int>> buildEdgeToCell(MeshType &mesh_in,
                                                  std::vector<std::array<int, 2>> &edges) {

        Mesh<MeshType> mesh(mesh_in);
        auto nodeToCell = buildNodeToCell(mesh_in);
        std::vector<std::vector<int>> e2c(edges.size());

        for (unsigned int edgeId = 0; edgeId < edges.size(); edgeId++) {
            std::array<int, 2> edge = edges[edgeId];
            auto sharedCells = getSharedCells(edge[0], edge[1], nodeToCell);
            e2c[edgeId] = sharedCells;
        }
        return e2c;
    }

    template<typename T>
    std::vector<std::vector<int> > buildCellToCell(T &meshInterface) {
        Mesh<T> mesh(meshInterface);
        printf("Parfait-buildCellToCell: putting cells in ADT for searching\n");
        Adt3DExtent adt = AdtBuilder::putCellsInAdt(meshInterface);

        std::vector<std::vector<int> > c2c;
        c2c.resize(mesh.numberOfCells());

        printf("Parfait-buildCellToCell: populating c2c connectivity\n");
        // populate c2c connectivity
        for (auto cell:mesh.cells()) {
            for (int neighborId:adt.retrieve(ExtentBuilder::build(cell))) {
                if (neighborId == cell.Id())
                    continue;
                for (auto face:cell) {
                    for (auto nbrFace : mesh.cell(neighborId))
                        if (facesMatch(face, nbrFace)) {
                            insertUnique(c2c[cell.Id()], neighborId);
                            insertUnique(c2c[neighborId], cell.Id());
                            break;
                        }
                }
            }
        }
        return c2c;
    }
}