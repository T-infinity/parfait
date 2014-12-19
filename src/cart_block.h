#ifndef CART_BLOCK_H
#define CART_BLOCK_H

//#include "box.h"
#include "extent.h"
#include <vector>
#include <assert.h>
#include <stdio.h>

class CartBlock : public Extent
{
    public:
        CartBlock();
        CartBlock(const double min_xyz[3], const double max_xyz[3]);
        CartBlock(const double extent_i[6]);
        CartBlock(const double min_xyz[3], const double max_xyz[3], 
                int ncells_x, int ncells_y, int ncells_z);
        CartBlock(const double extent[6],
                int ncells_x, int ncells_y, int ncells_z);

        void setDimensions(int ncells_x,int ncells_y,int ncells_z);

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

        int              getIdOfContainingCell(double point[3]) const;
        std::vector<int> getCellIdsInExtent(const Extent &b) const;
        //void   get_node_ids_in_box(Box &b, std::vector<int> &node_ids); // not implemented yet
        //
        void   getNode(int node, double point[3]) const;              
        void   getCellCentroid(int cell_id,double centroid[3]) const;
        Extent    createExtentFromCell(int cell) const;

        int convert_ijk_ToCellId(int i,int j,int k) const;
        int convert_ijk_ToNodeId(int i,int j,int k) const;
        void convertCellIdTo_ijk(int cell_id,int &i,int &j,int &k) const;
        void convertNodeIdTo_ijk(int node_id,int &i,int &j,int &k) const;

    private:
        int number_of_cells;
        int kx, ky, kz;

};

#endif
