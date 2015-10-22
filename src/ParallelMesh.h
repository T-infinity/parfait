#ifndef PARFAIT_MESHBASICPARALLEL_H
#define PARFAIT_MESHBASICPARALLEL_H

#include <memory>
#include <algorithm>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"

namespace Parfait {
    class ParallelMesh {
        public:
        ParallelMesh();
        std::shared_ptr<MeshConnectivity> connectivity;
        std::shared_ptr<MeshParallelMetaData> metaData;
        int countNodesAtDegree(int degree) const;
    };

  inline int ParallelMesh::countNodesAtDegree(int degree) const {
      return std::count(metaData->nodeOwnershipDegree.begin(),metaData->nodeOwnershipDegree.end(),degree);
  }

  inline ParallelMesh::ParallelMesh()
  : connectivity(std::make_shared<MeshConnectivity>()),
    metaData(std::make_shared<MeshParallelMetaData>()) {
  }
}

#endif