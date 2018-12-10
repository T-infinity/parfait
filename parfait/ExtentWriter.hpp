
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
#include <utility>

#include <utility>

#include <utility>

#include <vector>
#include <string>
#include <stdio.h>
#include <parfait/ByteSwap.h>
#include <parfait/VTKWriter.h>
#include <numeric>
#include "Extent.h"

namespace Parfait {

    inline void visualizeExtents(std::string filename, const std::vector<Parfait::Extent<double>> &extents) {

        auto get_point = [&](int node_id, double* p_out){
            int extent_index = node_id / 8;
            int point_index = node_id % 8;
            auto points = extents[extent_index].corners();
            auto p = points[point_index];
            p_out[0] = p[0];
            p_out[1] = p[1];
            p_out[2] = p[2];
        };

        auto get_cell_type = [](int cell_id){
            int hex = 12;
            return hex;
        };

        auto get_cell_nodes = [](int cell_id, int* node_ids){
            for(int i = 0; i < 8; i++){
                int node_id = 8*cell_id + i;
                node_ids[i] = node_id;
            }
        };

        Parfait::vtk::Writer writer(std::move(filename), 8*extents.size(), extents.size(),
                get_point, get_cell_type, get_cell_nodes);
        writer.visualize();
    }

    inline void visualizeExtents(std::string filename, const std::vector<Parfait::Extent<double>> &extents, const std::vector<double>& field) {
        auto get_point = [&](int node_id, double* p_out){
            int extent_index = node_id / 8;
            int point_index = node_id % 8;
            auto points = extents[extent_index].corners();
            auto p = points[point_index];
            p_out[0] = p[0];
            p_out[1] = p[1];
            p_out[2] = p[2];
        };

        auto get_cell_type = [](int cell_id){
            int hex = 12;
            return hex;
        };

        auto get_cell_nodes = [](int cell_id, int* node_ids){
            for(int i = 0; i < 8; i++){
                long node_id = 8*cell_id + i;
                node_ids[i] = node_id;
            }
        };

        auto get_cell_data = [&](int cell_id){
            return field[cell_id];
        };

        Parfait::vtk::Writer writer(std::move(filename), 8*extents.size(), extents.size(),
                                    get_point, get_cell_type, get_cell_nodes);
        writer.addCellField("some-field", get_cell_data);
        writer.visualize();
    }
}
