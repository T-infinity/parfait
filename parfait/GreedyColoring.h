
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
#include <set>
#include <vector>

class GreedyColoring {
public:
    static std::vector<int> colorGraph(const std::vector<std::vector<int>> &graph) {
        std::vector<int> colors(graph.size(), -1);
        for (int i = 0; i < graph.size(); i++) {
            std::set<int> nbr_colors;
            for (int nbr:graph[i])
                nbr_colors.insert(colors[nbr]);
            colors[i] = determineLowestAvailableColor(nbr_colors);
        }
        return colors;
    }

private:
    static int determineLowestAvailableColor(const std::set<int> &nbr_colors) {
        for (int i = 0; i < nbr_colors.size(); i++)
            if (nbr_colors.count(i) == 0)
                return i;
        return int(nbr_colors.size());
    }
};
