#ifndef PARFAIT_PARALLELPARTITIONABLEMESH_H
#define PARFAIT_PARALLELPARTITIONABLEMESH_H

#include <memory>
#include "ParallelMesh.h"

namespace Parfait {
  class ParallelPartitionableMesh : public PartitionableMesh {
  public:
      inline ParallelPartitionableMesh(std::shared_ptr<ParallelMesh> mesh) :
              PartitionableMesh(mesh),
              nodeOwnershipDegree(mesh->metaData->nodeOwnershipDegree),
              globalNodeIds(mesh->metaData->globalNodeIds){}

      inline bool doOwnLocalId(int localId) const {
          return nodeOwnershipDegree[localId] == 0;
      }
      inline long getGlobalNodeId(int localId) const {
          return globalNodeIds[localId];
      }
  private:
      std::vector<int>& nodeOwnershipDegree;
      std::vector<long>& globalNodeIds;
  };
}

#endif 
