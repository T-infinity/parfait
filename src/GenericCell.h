#ifndef GENERIC_CELL_H
#define GENERIC_CELL_H
#include "GenericFace.h"

#include <vector>
namespace Parfait {
    template<class MeshType>
    class Cell {
    public:
        Cell(MeshType &mesh, int cellId);
        int numberOfNodes() const;
        int numberOfFaces() const;
        std::vector<int> getNodes() const;
        void getNode(int i, double *p) const;

        int Id() const { return cellId; }

        class Iterator {
        public:
            Iterator(Cell<MeshType> &cell, int index);
            void operator++();
            bool operator!=(const Iterator &rhs);
            CellFace<MeshType> operator*();

        private:
            Cell<MeshType> &cell;
            int index;
        };

        Iterator begin() { return Iterator(*this, 0); }

        Iterator end() { return Iterator(*this, mesh.numberOfFacesInCell(cellId)); }

        CellFace<MeshType> getFace(int faceId);

    private:
        MeshType &mesh;
        int cellId;
    };
}
#include "GenericCell.hpp"

#endif
