
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
#include "NodeToNode.h"

namespace Parfait {
  template<class Mesh>
  class ParallelNodeToNodeBuilder {
  public:
      ParallelNodeToNodeBuilder(const Mesh& mesh);
      std::vector<std::vector<long>> buildNodeToNodeConnectivity();
  private:
      const Mesh& mesh;
      std::vector<std::vector<long>> appendGhostNodes(std::vector<std::vector<int>>& local_node_to_node);
  };

  template<class Mesh>
  std::vector<std::vector<long>> ParallelNodeToNodeBuilder<Mesh>::buildNodeToNodeConnectivity() {
      NodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
      auto local_node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
      return appendGhostNodes(local_node_to_node);
  }

  template<class Mesh>
  std::vector<std::vector<long>>
  ParallelNodeToNodeBuilder<Mesh>::appendGhostNodes(std::vector<std::vector<int>>& local_node_to_node) {
      std::vector<std::vector<long>> node_to_node;
      int index = 0;
      for (unsigned int localId = 0; localId < local_node_to_node.size(); localId++) {
          if (mesh.isGhostNode(localId)) continue;
          node_to_node.push_back({});
          for (int id : local_node_to_node[localId]) {
              node_to_node[index].push_back(mesh.getGlobalNodeId(id));
          }
          index++;
      }
      return node_to_node;
  }

  template<class Mesh>
  ParallelNodeToNodeBuilder<Mesh>::ParallelNodeToNodeBuilder(const Mesh& mesh_in) : mesh(mesh_in) {
  }
}
