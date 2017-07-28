#pragma once

#include <stdio.h>
#include <algorithm>
#include <assert.h>
#include <algorithm>
#include <array>

#include "VectorTools.h"
#include "GenericMeshTools.h"
#include "Adt3dExtent.h"
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
}