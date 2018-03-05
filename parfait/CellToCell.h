
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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