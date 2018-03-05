
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
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include "MapbcReader.h"
#include "Point.h"

//-------------------------------------------------------------
// This serves as an example of a mesh format that meets the
// criteria of a generic mesh that can be used with any of
// the templated tools.
// ------------------------------------------------------------
namespace Parfait {
    class ImportedUgrid {
    public:
        // Any Generic mesh must implement the following
        // functions (no inheritance required)
        void getNode(int nodeId, double node[3]) const;
        Point<double> getPoint(int nodeId) const;
        Point<double> getNode(int nodeId) const;
        int numberOfNodes() const;
        int numberOfCells() const;
        int numberOfNodesInCell(int id) const;
        int numberOfFacesInCell(int id) const;
        int numberOfNodesInCellFace(int cellId, int faceId) const;

        std::vector<int> getNodesInCellFace(int cellId, int faceId) const;

        std::vector<int> getNodesInCell(int cellId) const;
        void getNodesInCell(int cellId, int *cell) const;
        std::vector<int> getVtkOrderedNodesInCell(int cellId) const;

        void setNode(int nodeId, double node[3]);

        // Any Generic Surface mesh must implement the following
        int numberOfBoundaryFaces() const;
        int numberOfNodesInBoundaryFace(int boundaryFaceId) const;

        std::vector<int> getNodesInBoundaryFace(int boundaryFaceId) const;
        void getNodesInBoundaryFace(int boundaryFaceId, int *face) const;

        // Everything below is specific to ImportedUgrid and
        // has nothing to do with any other generic mesh
        // (i.e., this stuff doesn't have to be in your mesh)

        int getBoundaryTag(int boundaryFaceId) const;

        enum {TET = 0, PYRAMID = 1, PRISM = 2, HEX = 3};
        int getCellType(int cellId) const;

        ImportedUgrid(std::vector<double> nodes, std::vector<int> triangles,
                      std::vector<int> quads, std::vector<int> tets, std::vector<int> pyramids,
                      std::vector<int> prisms, std::vector<int> hexs,
                      std::vector<int> triangleTags, std::vector<int> quadTags);
        std::vector<double> nodes;
        std::vector<int> cellMap;
        std::vector<int> faceMap;
        std::vector<int> triangles;
        std::vector<int> quads;
        std::vector<int> tets;
        std::vector<int> pyramids;
        std::vector<int> prisms;
        std::vector<int> hexs;
        std::vector<int> triangleTags;
        std::vector<int> quadTags;
        std::vector<int> associated_component_id;
    protected:
        int nnodes;
        int ncells;
        int nfaces;
    };

}

#include "ImportedUgrid.hpp"