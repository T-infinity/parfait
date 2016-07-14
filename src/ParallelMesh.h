#pragma once
#include <memory>
#include <algorithm>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"
#include "SendReceivePair.h"
#include "Point.h"

namespace Parfait {
  class ParallelMeshBuilder;

  class ParallelMesh {
  public:
      friend class ParallelMeshBuilder;

      ParallelMesh(ParallelMeshBuilder builder);
      int numberOfNodes() const;
      int getTriangleTag(int id) const;
      int getQuadTag(int id) const;
      int getNodeComponentId(int id) const;
      long getGlobalNodeId(int id) const;
      int isGhostNode(int id) const;
      int countNodesAtDegree(int degree) const;
      int countNodesAboveDegree(int degree) const;
      Point<double> getXyz(int nodeId) const;

      int numberOfTets() const;
      const int* getTet(int id) const;
      int numberOfPyramids() const;
      const int* getPyramid(int id) const;
      int numberOfPrisms() const;
      const int* getPrism(int id) const;
      int numberOfHexes() const;
      const int* getHex(int id) const;
      int numberOfTriangles() const;
      const int* getTriangle(int id) const;
      int numberOfQuads() const;
      const int* getQuad(int id) const;

  private:
      std::shared_ptr<MeshParallelMetaData> metaData;
      std::shared_ptr<MeshConnectivity> connectivity;
  };


  class ParallelMeshBuilder{
  public:
      ParallelMeshBuilder();
      ParallelMeshBuilder(ParallelMesh mesh);
      std::shared_ptr<ParallelMesh> exportMesh();

      std::shared_ptr<MeshParallelMetaData> metaData;
      std::shared_ptr<MeshConnectivity> connectivity;
  };
}

#include "ParallelMesh.hpp"