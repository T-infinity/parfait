#ifndef PARFAIT_EXPLODEDMESH_HPP
#define PARFAIT_EXPLODEDMESH_HPP

#include <map>
#include "VectorTools.h"
#include "GenericMeshTools.h"

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
  void ExplodedMesh::shrinkPoints(double scale){
      for(int cellId = 0; cellId < (int)cells.size(); cellId++){
          auto center = GenericMeshTools::cellCenter(*this, cellId);
          for(int n : getNodesInCell(cellId)){
              auto p = points[n];
              auto dp = p - center;
              dp *= scale;
              points[n] = center + dp;
          }
      }
  }
}
#endif
