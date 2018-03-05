
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
#include "Point.h"

#include "CartBlock.h"
#include <vector>

namespace Parfait {
    class CartMesh {
    public:
        CartMesh(
                const Point<double> &lo,
                const Point<double> &hi,
                int numCellsX,
                int numCellsY,
                int numCellsZ
        );

        int numberOfCells() const { return block.numberOfCells(); }

        int numberOfNodes() const;
        int numberOfNodesInCell(int Id) const;
        int numberOfNodesInCellFace(int cellId, int faceId) const;
        int numberOfFacesInCell(int cellId) const;

        std::vector<int> getNodesInCellFace(int cellId, int faceId) const;
        std::vector<int> getNodesInCell(int cellId) const;

        int numberOfBoundaryFaces() const;
        int faceNeighbor(int cellId, int faceId) const;
        int faceBoundaryTag(int cellId, int faceId) const;
        bool isFaceBoundary(int cellId, int faceId) const;
        Point<double> faceArea(int cellId, int faceId) const;

        Point<double> cellCentroid(int cellId) const;
        double cellVolume(int cellId) const;

        void getNode(int nodeId, double node[3]) const;
        void getNode(int i, int j, int k, double node[3]) const;
        Point<double> getNode(int nodeId) const;
        int getNodeId(int i, int j, int k) const;
        void getLogicalDimensions(int &nX, int &nY, int &nZ) const;
        void getSpacings(double &hx, double &hy, double &hz) const;
        void getLogicalCoordinates(int nodeId, int &i, int &j, int &k) const;

    private:
        CartBlock block;
        double volume;

        int numberOfCellsOnSide(int faceId) const;
        int whatSideIsCellFaceOn(int cellId, int faceId) const;
    };


}

#include "CartMesh.hpp"