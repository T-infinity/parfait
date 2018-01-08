#pragma once
#include <memory>
#include <algorithm>
#include <parfait/Point.h>
#include <MessagePasser/MessagePasser.h>

namespace Parfait {
  class ParallelMeshBuilder;

  struct ParallelMeshData {
      std::vector<double> xyz;
      std::vector<int> triangleTags;
      std::vector<int> quadTags;

      std::vector<long> globalNodeIds;
      std::vector<int> nodeComponentIds;
      std::vector<int> nodeOwnerRank;

      std::vector<int> triangles;
      std::vector<int> quads;
      std::vector<int> tets;
      std::vector<int> pyramids;
      std::vector<int> prisms;
      std::vector<int> hexs;
  };

  class ParallelMesh {
  public:
      friend class ParallelMeshBuilder;

      ParallelMesh(MessagePasser messagePasser, ParallelMeshBuilder builder);
      int numberOfNodes() const;
      int numberOfCells() const;
      int numberOfNodesInCell(int id) const;
      const int* getCell(int id) const;
      std::vector<int> getNodesInCell(int id) const;
      void getNodesInCell(int id,int* cell) const;
      std::vector<int> getNodesInFace(int id) const;
      int getTriangleTag(int id) const;
      int owningRankOfNode(int id) const;
      int getQuadTag(int id) const;
      int getNodeComponentId(int id) const;
      long getGlobalNodeId(int id) const;
      int isGhostNode(int id) const;
      int countOwnedNodes() const;
      Point<double> getXyz(int nodeId) const;

      int numberOfTets() const;
      const int* getTet(int id) const;
      int numberOfPyramids() const;
      const int* getPyramid(int id) const;
      int numberOfPrisms() const;
      const int* getPrism(int id) const;
      int numberOfHexes() const;
      const int* getHex(int id) const;
      int numberOfFaces() const;
      int numberOfNodesInFace(int id) const;
      const int* getFace(int id) const;
      int getFaceTag(int id) const;
      int numberOfTriangles() const;
      const int* getTriangle(int id) const;
      int numberOfQuads() const;
      const int* getQuad(int id) const;

      MessagePasser messagePasser;
      std::shared_ptr<ParallelMeshData> data;
  };

  class ParallelMeshBuilder {
  public:
      ParallelMeshBuilder(MessagePasser mp);
      ParallelMeshBuilder(MessagePasser mp, ParallelMesh mesh);
      std::shared_ptr<ParallelMesh> exportMesh();

      MessagePasser messagePasser;
      std::shared_ptr<ParallelMeshData> data;
  };
}

#include "ParallelMesh.hpp"
