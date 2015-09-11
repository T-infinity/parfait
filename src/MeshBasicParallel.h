#ifndef PARFAIT_MESHBASICPARALLEL_H
#define PARFAIT_MESHBASICPARALLEL_H

#include <memory>
#include "MeshConnectivity.h"
#include "MeshParallelMetaData.h"

namespace Parfait {
    class MeshBasicParallel {
        public:
        MeshBasicParallel();
        std::shared_ptr<MeshConnectivity> connectivity;
        std::shared_ptr<MeshParallelMetaData> metaData;
        int numberOfNodesAtDegree(int degree) const;
    };

  inline int MeshBasicParallel::numberOfNodesAtDegree(int degree) const {
      int count = 0;
      for(auto d : metaData->nodeOwnershipDegree)
          if(d == 0)
              count++;
      return count;
  }
  inline MeshBasicParallel::MeshBasicParallel()
  : connectivity(std::make_shared<MeshConnectivity>()),
    metaData(std::make_shared<MeshParallelMetaData>()) {
  }
}

#endif
