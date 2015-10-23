#ifndef MESH_SLICE_H
#define MESH_SLICE_H

#include <vector>
#include "GenericMesh.h"
#include "ExtentBuilder.h"

#include <exception>
#include <stdexcept>

namespace Parfait {
    template<typename MeshType>
    class MeshSlice {
    public:
        MeshSlice(MeshType &mesh_in, const Extent<double> &domain_in) :
                mesh(mesh_in),
                domain(domain_in) {
            Init();
        }

        template<typename T>
        MeshSlice(MeshType &mesh_in, bool isNodeData,
                  T *data, int blocksize, int offset,
                  T lo, T hi) : mesh(mesh_in) {
            globalToLocal_cellId.resize(mesh.numberOfCells(), -1);
            globalToLocal_nodeId.resize(mesh.numberOfNodes(), -1);

            Mesh<MeshType> genMesh(mesh);

            if (isNodeData) {
                throw std::logic_error("MeshSlice threshold only supports cell "
                                               "data at this time.");
            }

            int cellId = 0;
            for (auto cell : genMesh.cells()) {
                T item = data[blocksize * cell.Id() + offset];
                if (item > lo && item < hi) {
                    globalToLocal_cellId[cell.Id()] = cellId++;
                    for (int node : cell.getNodes()) {
                        globalToLocal_nodeId[node] = 1; // Just turn it on.
                    }
                }
            }
            num_cells = cellId;

            int nodeId = 0;
            for (int node = 0; node < genMesh.numberOfNodes(); node++) {
                if (globalToLocal_nodeId[node] == 1) {
                    globalToLocal_nodeId[node] = nodeId++;
                }
            }
            num_nodes = nodeId;

            localToGlobal_cellId.resize(num_cells, -1);
            localToGlobal_nodeId.resize(num_nodes, -1);

            for (unsigned int index = 0; index < globalToLocal_cellId.size(); index++) {
                int localId = globalToLocal_cellId[index];
                if (localId != -1) {
                    localToGlobal_cellId[localId] = index;
                }
            }

            for (unsigned int index = 0; index < globalToLocal_nodeId.size(); index++) {
                int localId = globalToLocal_nodeId[index];
                if (localId != -1) {
                    localToGlobal_nodeId[localId] = index;
                }
            }
        }

        int numberOfNodes() const;
        int numberOfCells() const;

        int numberOfNodesInCell(int cellId) const;
        int numberOfFacesInCell(int cellId) const;
        int numberOfNodesInCellFace(int cellId, int faceId) const;
        std::vector<int> getNodesInCell(int cellId) const;
        std::vector<int> getNodesInCellFace(int cellId, int faceId) const;


        void getNode(int nodeId, double node[3]) const;

        std::vector<int> globalToLocal_cellId;
        std::vector<int> globalToLocal_nodeId;

        std::vector<int> localToGlobal_cellId;
        std::vector<int> localToGlobal_nodeId;
    private:
        MeshType &mesh;
        Extent<double> domain;

        int num_nodes, num_cells;

        void Init();
    };

    template<typename MeshType>
    int MeshSlice<MeshType>::numberOfNodes() const { return num_nodes; }

    template<typename MeshType>
    int MeshSlice<MeshType>::numberOfCells() const { return num_cells; }

    template<typename MeshType>
    int MeshSlice<MeshType>::numberOfNodesInCell(
            int cellId) const { return mesh.numberOfNodesInCell(localToGlobal_cellId[cellId]); }

    template<typename MeshType>
    int MeshSlice<MeshType>::numberOfFacesInCell(
            int cellId) const { return mesh.numberOfFacesInCell(localToGlobal_cellId[cellId]); }

    template<typename MeshType>
    int MeshSlice<MeshType>::numberOfNodesInCellFace(int cellId, int faceId) const {
        return mesh.numberOfNodesInCellFace(localToGlobal_cellId[cellId], faceId);
    }

    template<typename MeshType>
    std::vector<int> MeshSlice<MeshType>::getNodesInCellFace(int cellId, int faceId) const {
        auto nodes = mesh.getNodesInCellFace(localToGlobal_cellId[cellId], faceId);
        for (int &node : nodes) {
            node = globalToLocal_nodeId[node];
        }
        return nodes;
    }

    template<typename MeshType>
    std::vector<int> MeshSlice<MeshType>::getNodesInCell(int cellId) const {
        auto nodes = mesh.getNodesInCell(localToGlobal_cellId[cellId]);
        for (int &node : nodes) {
            node = globalToLocal_nodeId[node];
        }
        return nodes;
    }

    template<typename MeshType>
    void MeshSlice<MeshType>::getNode(int nodeId, double node[3]) const {
        mesh.getNode(localToGlobal_nodeId[nodeId], node);
    }


    template<typename MeshType>
    void MeshSlice<MeshType>::Init() {

        globalToLocal_cellId.resize(mesh.numberOfCells(), -1);
        globalToLocal_nodeId.resize(mesh.numberOfNodes(), -1);

        Mesh<MeshType> genMesh(mesh);

        int cellId = 0;
        for (auto cell : genMesh.cells()) {
            auto cellExtent = ExtentBuilder::build(cell);
            if (domain.contains(cellExtent)) {
                globalToLocal_cellId[cell.Id()] = cellId++;
                for (int node : cell.getNodes()) {
                    globalToLocal_nodeId[node] = 1; // Just turn it on.
                }
            }
        }

        num_cells = cellId;

        int nodeId = 0;
        for (int node = 0; node < genMesh.numberOfNodes(); node++) {
            if (globalToLocal_nodeId[node] == 1) {
                globalToLocal_nodeId[node] = nodeId++;
            }
        }
        num_nodes = nodeId;

        localToGlobal_cellId.resize(num_cells, -1);
        localToGlobal_nodeId.resize(num_nodes, -1);

        for (int index = 0; index < (int)globalToLocal_cellId.size(); index++) {
            int localId = globalToLocal_cellId[index];
            if (localId != -1) {
                localToGlobal_cellId[localId] = index;
            }
        }

        for (unsigned int index = 0; index < globalToLocal_nodeId.size(); index++) {
            int localId = globalToLocal_nodeId[index];
            if (localId != -1) {
                localToGlobal_nodeId[localId] = index;
            }
        }
    }
}
#endif
