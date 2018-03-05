
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

namespace Parfait {
  class ExplodedMesh {
  public:
      template<class Mesh>
      ExplodedMesh(const Mesh &mesh);
      int numberOfCells() const;
      int numberOfNodes() const;

      void getNode(int nodeId, double node[3]) const;
      Point<double> getPoint(int nodeId) const;
      std::vector<int> getNodesInCell(int cellId) const;
      int numberOfNodesInCell(int cellId) const;
      int numberOfFacesInCell(int cellId) const;
      int numberOfNodesInCellFace(int cellId, int faceId) const;
      std::vector<int> getNodesInCellFace(int cellId, int faceId) const;

  private:
      typedef std::vector<std::vector<int>> ExplodedCell;
      std::vector<ExplodedCell> cells;
      std::vector<Point<double>> points;
      std::vector<int> newNodeIdToOldNodeId;
      template<class Mesh>
      void copyCells(const Mesh &mesh);
      template<class Mesh>
      void duplicatePoints(const Mesh &mesh);
      void shrinkPoints(double scale);
  };
}

#include "ExplodedMesh.hpp"