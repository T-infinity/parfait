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