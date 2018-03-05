
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
#include "VolumeMeshInterface.h"
#include <functional>

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