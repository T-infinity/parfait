
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

#include "Extent.h"
#include <vector>
#include <assert.h>
#include <stdio.h>

namespace Parfait {
    class CartBlock : public Extent<double> {
    public:
        CartBlock() = delete;
        CartBlock(const Point<double>& min_xyz, const Point<double>& max_xyz,
                  int ncells_x, int ncells_y, int ncells_z);
        CartBlock(const Extent<double>& extent,
                  int ncells_x, int ncells_y, int ncells_z);

        double get_dx() const;
        double get_dy() const;
        double get_dz() const;

        int numberOfCells_X() const;
        int numberOfCells_Y() const;
        int numberOfCells_Z() const;
        int numberOfNodes_X() const;
        int numberOfNodes_Y() const;
        int numberOfNodes_Z() const;

        int numberOfCells() const;
        int numberOfNodes() const;

        std::vector<int> getNodesInCell(int id) const;

        int getIdOfContainingCell(const double point[3]) const;
        std::vector<int> getCellIdsInExtent(const Extent<double> &search_extent) const;
        Extent<int> getRangeOfOverlappingCells(const Extent<double> &search_extent) const;
        void getNode(int node, double point[3]) const;
        Point<double> getPoint(int node_id) const;
        void getCellCentroid(int cell_id, double centroid[3]) const;
        Extent<double> createExtentFromCell(int cell) const;

        int convert_ijk_ToCellId(int i, int j, int k) const;
        int convert_ijk_ToNodeId(int i, int j, int k) const;
        void convertCellIdTo_ijk(int cell_id, int &i, int &j, int &k) const;
        void convertNodeIdTo_ijk(int node_id, int &i, int &j, int &k) const;

    private:
        int number_of_cells;
        int kx, ky, kz;
    };

}

#include "CartBlock.hpp"