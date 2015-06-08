#ifndef GENERIC_FACE_H
#define GENERIC_FACE_H

#include <vector>
namespace Parfait {
    template<class MeshType>
    class CellFace {
    public:
        CellFace(MeshType &mesh, int cellId, int faceId);
        int numberOfNodes() const;
        std::vector<int> getNodes() const;
        void getNode(int nodeId, double *p) const;

        int Id() { return faceId; }

        class Iterator {
        public:
            Iterator(CellFace<MeshType> &face, int index);
            void operator++();
            bool operator!=(const Iterator &rhs);
            int  operator*();
        private:
            CellFace<MeshType> &face;
            int index;
        };

        Iterator begin() { return Iterator(*this, 0); }

        Iterator end() { return Iterator(*this, mesh.numberOfNodesInCellFace(cellId, faceId)); }

        int cellId;
        int faceId;
    private:
        MeshType &mesh;
    };
}
#include "generic_face.hpp"

#endif
