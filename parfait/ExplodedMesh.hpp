
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

#include <map>
#include "VectorTools.h"

namespace Parfait{
  template <class Mesh>
  ExplodedMesh::ExplodedMesh(const Mesh &mesh){
      copyCells(mesh);
      duplicatePoints(mesh);
      shrinkPoints(0.75);
  }
  template <class Mesh>
  void ExplodedMesh::copyCells(const Mesh &mesh) {
      cells.resize(mesh.numberOfCells());
      for(int cellId = 0; cellId < mesh.numberOfCells(); cellId++){
            int num_faces = mesh.numberOfFacesInCell(cellId);
            cells[cellId].resize(num_faces);
            for(int face = 0; face < num_faces; face++){
                cells[cellId][face] = mesh.getNodesInCellFace(cellId, face);
            }
        }
  }

  template <class Mesh>
  void ExplodedMesh::duplicatePoints(const Mesh &mesh) {
      int numberOfDuplicatedPoints = 0;
      for(int cellId = 0; cellId < mesh.numberOfCells(); cellId++){
          numberOfDuplicatedPoints += mesh.numberOfNodesInCell(cellId);
      }
      points.resize(numberOfDuplicatedPoints);

      newNodeIdToOldNodeId.resize(numberOfDuplicatedPoints);
      int newNodeId = 0;
      for(int cellId = 0; cellId < mesh.numberOfCells(); cellId++){
          auto cellNodes = mesh.getNodesInCell(cellId);
          std::map<int, int> oldToNew;
          for(int old : cellNodes){
              oldToNew[old] = newNodeId;
              newNodeIdToOldNodeId[newNodeId] = old;
              newNodeId++;
          }
          for(int face = 0; face < (int)cells[cellId].size(); face++){
              for(auto &newNode : cells[cellId][face]){
                  newNode = oldToNew[newNode];
              }
          }
      }

      for(unsigned int node = 0; node < points.size(); node++){
          int oldId = newNodeIdToOldNodeId[node];
          points[node] = mesh.getNode(oldId);
      }
  }

  int ExplodedMesh::numberOfCells() const {
      return cells.size();
  }
  int ExplodedMesh::numberOfNodes() const {
      return points.size();
  }

  Point<double> ExplodedMesh::getPoint(int nodeId) const{
      Point<double> p;
      getNode(nodeId, p.data());
      return p;
  }

  void ExplodedMesh::getNode(int nodeId, double node[3]) const{
      node[0] = points[nodeId][0];
      node[1] = points[nodeId][1];
      node[2] = points[nodeId][2];
  }

  std::vector<int> ExplodedMesh::getNodesInCell(int cellId) const{
      std::vector<int> nodes;
      for(unsigned int face = 0; face < cells[cellId].size(); face++){
          for(int n : cells[cellId][face]){
              insertUnique(nodes, n);
          }
      }
      return nodes;
  }

  int ExplodedMesh::numberOfNodesInCell(int cellId) const{
      return getNodesInCell(cellId).size();
  }

  int ExplodedMesh::numberOfFacesInCell(int cellId) const{
      return cells[cellId].size();
  }
  int ExplodedMesh::numberOfNodesInCellFace(int cellId, int faceId) const{
      return cells[cellId][faceId].size();
  }
  std::vector<int> ExplodedMesh::getNodesInCellFace(int cellId, int faceId) const{
      return cells[cellId][faceId];
  }
  template<class MeshType, class container>
  Point<double> computeCenter(MeshType &mesh, const container &nodes) {
      Point<double> center(0, 0, 0);
      for (auto node : nodes) {
          Point<double> a;
          mesh.getNode(node, &a[0]);
          center += a;
      }

      center /= (double) nodes.size();
      return center;
  }

  void ExplodedMesh::shrinkPoints(double scale){
      for(int cellId = 0; cellId < (int)cells.size(); cellId++){
          auto center = computeCenter(*this, this->getNodesInCell(cellId));
          for(int n : getNodesInCell(cellId)){
              auto p = points[n];
              auto dp = p - center;
              dp *= scale;
              points[n] = center + dp;
          }
      }
  }
}
