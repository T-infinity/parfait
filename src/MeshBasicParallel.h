#ifndef PARFAIT_MESHBASICPARALLEL_H
#define PARFAIT_MESHBASICPARALLEL_H

#include <memory>
#include <algorithm>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"

namespace Parfait {
    class MeshBasicParallel {
        public:
        MeshBasicParallel();
        std::shared_ptr<MeshConnectivity> connectivity;
        std::shared_ptr<MeshParallelMetaData> metaData;
        int countNodesAtDegree(int degree) const;
    };

  inline int MeshBasicParallel::countNodesAtDegree(int degree) const {
      return std::count(metaData->nodeOwnershipDegree.begin(),metaData->nodeOwnershipDegree.end(),degree);
  }

  inline MeshBasicParallel::MeshBasicParallel()
  : connectivity(std::make_shared<MeshConnectivity>()),
    metaData(std::make_shared<MeshParallelMetaData>()) {
  }
}

#endif
