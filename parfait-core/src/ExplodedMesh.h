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