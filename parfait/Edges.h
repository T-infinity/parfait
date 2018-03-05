
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
#include <array>
#include <set>
#include <vector>
#include "VectorTools.h"
#include "EdgeBuilder.h"

namespace Parfait {
  namespace Edges {

    template<class MeshType>
    std::vector<std::array<int, 2>> getUniqueEdges(MeshType &mesh_in) {

        int a, b;
        std::set<std::pair<int, int>> edgeSet;
        for (int i = 0; i < mesh_in.numberOfCells(); ++i) {
            auto cell = mesh_in.getNodesInCell(i);
            switch (cell.size()) {
                case 4:a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[0];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[0];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[1];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[1];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    break;
                case 5:a = cell[4];
                    b = cell[0];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[4];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[4];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[4];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[0];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    break;
                case 6:a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[1];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[0];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[3];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[4];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[5];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[0];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[1];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    break;
                case 8:a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[1];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[3];
                    b = cell[0];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[4];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[5];
                    b = cell[6];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[6];
                    b = cell[7];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[7];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[0];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[1];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[2];
                    b = cell[6];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    a = cell[3];
                    b = cell[7];
                    edgeSet.insert(std::make_pair(std::min(a, b), std::max(a, b)));
                    break;
                default:throw std::logic_error("Invalid cell");
            }
        }

        std::vector<std::array<int, 2>> edges;
        edges.reserve(edgeSet.size());
        for (auto edge : edgeSet) {
            std::array<int, 2> e = {edge.first, edge.second};
            edges.push_back(e);
        }

        return edges;
    }
  }
}
