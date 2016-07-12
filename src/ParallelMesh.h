#ifndef PARFAIT_MESHBASICPARALLEL_H
#define PARFAIT_MESHBASICPARALLEL_H

#include <memory>
#include <algorithm>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"
#include "SendReceivePair.h"
#include "Point.h"

namespace Parfait {
    class ParallelMesh {
        public:
        ParallelMesh();
        ParallelMesh(std::shared_ptr<MeshConnectivity> conn, std::shared_ptr<MeshParallelMetaData> mData);
        int numberOfNodes();
        int getTriangleTag(int id);
        int getQuadTag(int id);
        int getNodeComponentId(int id);
        long getGlobalNodeId(int id);
        int isGhostNode(int id);
        int countNodesAtDegree(int degree) const;
        int countNodesAboveDegree(int degree) const;
        Point<double> getXyz(int nodeId);

        std::shared_ptr<MeshConnectivity> connectivity;
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

    inline int ParallelMesh::numberOfNodes() {
        return metaData->xyz.size()/3;
    }

    inline Point<double> ParallelMesh::getXyz(int nodeId) {
        return Parfait::Point<double>(&metaData->xyz[3*nodeId]);
    }

    inline int ParallelMesh::getTriangleTag(int id) {
        return metaData->triangleTags[id];
    }

    inline int ParallelMesh::getQuadTag(int id) {
        return metaData->quadTags[id];
    }

    inline int ParallelMesh::getNodeComponentId(int id) {
        return metaData->nodeComponentIds[id];
    }

    inline long ParallelMesh::getGlobalNodeId(int id) {
        return metaData->globalNodeIds[id];
    }

    inline int ParallelMesh::isGhostNode(int id) {
        return metaData->nodeOwnershipDegree[id] == 0 ? 0 : 1;
    }

    inline ParallelMesh::ParallelMesh(std::shared_ptr<MeshConnectivity> conn, std::shared_ptr<MeshParallelMetaData> mData) {
        metaData = mData;
        connectivity = conn;
    }


}

#endif