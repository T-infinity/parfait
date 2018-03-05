
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
