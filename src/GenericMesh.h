#ifndef GENERIC_MESH_H
#define GENERIC_MESH_H
#include "GenericCell.h"
#include "GenericFace.h"
#include "GenericNode.h"
#include <vector>
namespace Parfait {
    template<class MeshType>
    class Mesh {
    public:
        Mesh(MeshType &mesh);

        int numberOfNodes() const { return mesh.numberOfNodes(); }

        int numberOfCells() const { return mesh.numberOfCells(); }

        void getNode(int nodeId, double node[3]) const { mesh.getNode(nodeId, node); }

        Cell<MeshType> cell(int cellId) const { return Cell<MeshType>(mesh, cellId); }

        int numberOfNodesInCell(int cellId) const { return mesh.numberOfNodesInCell(cellId); }

        void getNodesInCell(int cellId, int *nodes) { mesh.getNodesInCell(cellId, nodes); }

        std::vector<int> getNodesInCell(int cellId) { return mesh.getNodesInCell(cellId); }

        int numberOfFacesInCell(int cellId) const { return mesh.numberOfFacesInCell(cellId); }

        int numberOfNodesInCellFace(int cellId, int faceId) const {
            return mesh.numberOfNodesInCellFace(cellId, faceId);
        }

        class CellCollection {
        private:
            const Mesh<MeshType> &mesh;
        public:
            CellCollection(const Mesh<MeshType> &mesh);

            class Iterator {
            public:
                Iterator(const Mesh<MeshType> &mesh, int index);
                void operator++();
                bool operator!=(const Iterator &rhs);
                Cell<MeshType>  operator*();
            private:
                const Mesh<MeshType> &mesh;
                int index;
            };

            Iterator begin() const { return Iterator(mesh, 0); }

            Iterator end() const { return Iterator(mesh, mesh.numberOfCells()); }
        };

        CellCollection cells() const { return CellCollection(*this); }

    protected:
        MeshType &mesh;
    };
}
#include "GenericMesh.hpp"
#endif
