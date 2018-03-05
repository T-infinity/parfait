
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

namespace Parfait {
    class EdgeBuilder {
    public:
        template<typename T>
        void addCell(int *cell, const T &local_edge_map) {
            for (auto &edge:local_edge_map) {
                int a = std::min(cell[edge[0]], cell[edge[1]]);
                int b = std::max(cell[edge[0]], cell[edge[1]]);
                e.insert({a, b});
            }
        }

        std::vector<std::array<int, 2>> edges() {
            return std::vector<std::array<int, 2>>(e.begin(), e.end());
        };
    private:
        std::set<std::array<int, 2>> e;
    };
}
