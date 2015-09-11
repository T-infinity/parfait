#ifndef PARFAIT_PARALLELNODETONODE_H
#define PARFAIT_PARALLELNODETONODE_H

#include <vector>
#include "GenericMesh.h"
#include "NodeToNode.h"

namespace Parfait{
  template <class Mesh>
  class ParallelNodeToNodeBuilder {
  public:
      ParallelNodeToNodeBuilder(const Mesh& mesh);
      std::vector<std::vector<long>> buildNodeToNodeConnectivity();
  private:
      const Mesh& mesh;
      std::vector<std::vector<long>> node_to_node;
      std::vector<long> global_node_ids;
  };

  template <class Mesh>
  std::vector<std::vector<long>> ParallelNodeToNodeBuilder<Mesh>::buildNodeToNodeConnectivity() {
      NodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
      auto local_node_to_node = nodeBuilder.buildNodeToNodeConnectivity();
      int index = 0;
      for(int localId = 0; localId < local_node_to_node.size(); localId++){
          if(not mesh.doOwnLocalId(localId)) continue;
          global_node_ids.push_back(mesh.getGlobalNodeId(localId));
          node_to_node.push_back({});
          for(int id : local_node_to_node[localId]){
              node_to_node[index].push_back(mesh.getGlobalNodeId(id));
          }
          index++;
      }
    return node_to_node;
  }

  template <class Mesh>
  ParallelNodeToNodeBuilder<Mesh>::ParallelNodeToNodeBuilder(const Mesh &mesh_in) : mesh(mesh_in) {
  }
}
#endif 
