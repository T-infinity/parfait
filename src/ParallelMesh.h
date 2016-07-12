#ifndef PARFAIT_MESHBASICPARALLEL_H
#define PARFAIT_MESHBASICPARALLEL_H

#include <memory>
#include <algorithm>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"
#include "SendReceivePair.h"

namespace Parfait {
    class ParallelMesh {
        public:
        ParallelMesh();
        std::shared_ptr<MeshConnectivity> connectivity;
        int countNodesAtDegree(int degree) const;
        int countNodesAboveDegree(int degree) const;
        std::shared_ptr<MeshParallelMetaData> metaData;
    private:
    };

  inline int ParallelMesh::countNodesAtDegree(int degree) const {
      return std::count(metaData->nodeOwnershipDegree.begin(),metaData->nodeOwnershipDegree.end(),degree);
  }

  inline int ParallelMesh::countNodesAboveDegree(int degree) const {
      int count = 0;
      for(auto& o : metaData->nodeOwnershipDegree)
          if(o > degree)
              count++;
      return count;
  }

  inline ParallelMesh::ParallelMesh()
  : connectivity(std::make_shared<MeshConnectivity>()),
    metaData(std::make_shared<MeshParallelMetaData>()){
  }
}

#endif