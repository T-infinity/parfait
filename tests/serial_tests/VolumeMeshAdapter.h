#pragma once
#include "VolumeMeshInterface.h"

namespace Parfait {
    class VolumeMeshAdapter : public VolumeMeshInterface {
    private:
        std::function<int()> numberOfNodes_m;
        std::function<int()> numberOfCells_m;
        std::function<std::vector<int>(int)> getNodesInCell_m;
        std::function<Point<double>(int)> getPoint_m;

    public:
        template<class MeshType>
        VolumeMeshAdapter(const MeshType &mesh)
                : numberOfNodes_m(std::bind(&MeshType::numberOfNodes, std::ref(mesh))),
                  numberOfCells_m(std::bind(&MeshType::numberOfCells, std::ref(mesh))),
                  getNodesInCell_m(std::bind(&MeshType::getNodesInCell, std::ref(mesh),std::placeholders::_1)),
                  getPoint_m(std::bind(&MeshType::getPoint, std::ref(mesh), std::placeholders::_1))

        {
        }

        virtual int numberOfNodes() const override {
            return numberOfNodes_m();
        }

        virtual int numberOfCells() const override {
            return numberOfCells_m();
        }

        virtual std::vector<int>  getNodesInCell(int cellId) const override {
            return getNodesInCell_m(cellId);
        }

        virtual Point<double> getPoint(int nodeId) const override {
            return getPoint_m(nodeId);
        }
    };
}