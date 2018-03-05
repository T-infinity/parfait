
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
#include <MessagePasser/MessagePasser.h>
#include <parfait/ParallelImportedUgrid.h>
#include <parfait/ParallelMesh.h>

namespace Parfait {
  class NodeBasedRedistributor {
  public:
      NodeBasedRedistributor(MessagePasser mp,
                             std::shared_ptr<ParallelMesh> mesh, std::vector<int>& part);

      std::shared_ptr<ParallelMesh> redistribute();
  private:

      MessagePasser mp;
      std::vector<long> redistributeNodeIds();
      std::vector<int> redistributeTags(std::vector<long>& my_non_ghost_ids,
                                        std::vector<int>& cells, std::vector<int>& tags, int cellSize);
      std::vector<long> redistributeCells(std::vector<long>& my_non_ghost_ids,
                                          std::vector<int>& cells, int cellSize);
      std::vector<long> redistributeSurfaceCells(std::vector<long>& my_non_ghost_ids,
                                                 std::vector<int>& cells, int cellSize);

      std::vector<long> identifyGhostNodes(std::vector<long>& my_non_ghost_ids,
                                           std::vector<long>& recvTets,
                                           std::vector<long>& recvPyramids,
                                           std::vector<long>& recvPrisms,
                                           std::vector<long>& recvHexs);
      void redistributeNodeMetaData(std::vector<long>& my_non_ghost_ids,
                                    std::vector<long>& my_ghost_ids);
      std::shared_ptr<ParallelMeshBuilder> mesh;
      std::shared_ptr<ParallelMesh> mesh_old;
      std::vector<int> part;
      std::vector<int> nodeMap;
      std::vector<int> recvAssociatedComponentIds;
      std::vector<double> recvXYZ;

      int getLocalNodeId(long globalNodeId, std::vector<long>& my_non_ghost_ids,
                         std::vector<long>& my_ghost_ids);

      std::vector<int> convertToLocalIds(std::map<long, int> global_to_local_map, const std::vector<long>& ids) const;
      bool amItheOwnerOfThisNode(long globalId, const std::map<long, int>& globalToLocal);
      bool iShouldSendThisCell(int* cell, int cellSize, const std::vector<long>& neededNodeIds);
      bool iShouldSendThisSurfaceCell(int* cell, int cellSize, const std::vector<long>& neededNodeIds);
      std::map<long, std::vector<int>> mapNodesToCells(std::vector<long>& globalNodeIds,
                                                       std::vector<int>& cells, int cellSize);
  };
}
#include "NodeCenteredRedistributor.hpp"
