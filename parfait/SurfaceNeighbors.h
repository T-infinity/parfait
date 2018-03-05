
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
#include <vector>
#include <memory>
#include "ParallelMesh.h"

namespace Parfait {

    namespace SurfaceNeighbors {
        inline std::vector<std::vector<int>> buildNode2Cell(const std::shared_ptr<Parfait::ParallelMesh> &mesh) {
            std::vector<std::vector<int>> node2Cell(mesh->numberOfNodes());

            for (int cell_id = 0; cell_id < mesh->numberOfCells(); cell_id++) {
                int cell_length = mesh->numberOfNodesInCell(cell_id);
                auto cell = mesh->getCell(cell_id);
                for (int i = 0; i < cell_length; i++) {
                    int node = cell[i];
                    insertUnique(node2Cell[node], cell_id);
                }
            }
            return node2Cell;
        }

        inline int findNeighbor(const std::vector<std::vector<int>> &n2c, const int *cell, int cell_length) {
            auto neighbors = n2c[cell[0]];
            std::set<int> candidates(neighbors.begin(), neighbors.end());
            for (int i = 1; i < cell_length; i++) {
                auto cell_neighbors = n2c[cell[i]];
                for (auto c : neighbors) {
                    if (not isIn(cell_neighbors, c)) {
                        candidates.erase(c);
                    }
                }
            }
            if (candidates.size() != 1) {
                throw std::logic_error(
                        "Found an incorrect number of volume neighbors for surface element.  Should be 1, found: " +
                        std::to_string(candidates.size()));
            }
            return *candidates.begin();
        }

        inline std::vector<int> buildSurfaceNeighbors(const std::shared_ptr<Parfait::ParallelMesh> &mesh) {
            std::vector<int> neighbors;

            auto n2c = buildNode2Cell(mesh);

            neighbors.resize(mesh->numberOfTriangles() + mesh->numberOfQuads());
            for (int triangle = 0; triangle < mesh->numberOfTriangles(); triangle++) {
                auto cell = mesh->getTriangle(triangle);
                int cell_length = 3;
                neighbors[triangle] = findNeighbor(n2c, cell, cell_length);
            }
            for (int quad = 0; quad < mesh->numberOfQuads(); quad++) {
                auto cell = mesh->getQuad(quad);
                int cell_length = 4;
                neighbors[quad + mesh->numberOfTriangles()] = findNeighbor(n2c, cell, cell_length);
            }
            return neighbors;
        }
    }
}
