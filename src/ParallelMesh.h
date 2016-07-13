#pragma once

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
        int numberOfNodes() const;
        int getTriangleTag(int id) const;
        int getQuadTag(int id) const;
        int getNodeComponentId(int id) const;
        long getGlobalNodeId(int id) const;
        int isGhostNode(int id) const;
        int countNodesAtDegree(int degree) const;
        int countNodesAboveDegree(int degree) const;
        Point<double> getXyz(int nodeId) const;

        int numberOfTets() const;
        const int* getTet(int id) const;
        int numberOfPyramids() const;
        const int* getPyramid(int id) const;
        int numberOfPrisms() const;
        const int* getPrism(int id) const;
        int numberOfHexes() const;
        const int* getHex(int id) const;
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

    inline int ParallelMesh::numberOfNodes() const {
        return metaData->xyz.size()/3;
    }

    inline Point<double> ParallelMesh::getXyz(int nodeId) const {
        return Parfait::Point<double>(&metaData->xyz[3*nodeId]);
    }

    inline int ParallelMesh::getTriangleTag(int id) const {
        return metaData->triangleTags[id];
    }

    inline int ParallelMesh::getQuadTag(int id) const {
        return metaData->quadTags[id];
    }

    inline int ParallelMesh::getNodeComponentId(int id) const {
        return metaData->nodeComponentIds[id];
    }

    inline long ParallelMesh::getGlobalNodeId(int id) const {
        return metaData->globalNodeIds[id];
    }

    inline int ParallelMesh::isGhostNode(int id) const {
        return metaData->nodeOwnershipDegree[id] != 0;
    }

    inline ParallelMesh::ParallelMesh(std::shared_ptr<MeshConnectivity> conn, std::shared_ptr<MeshParallelMetaData> mData) {
        metaData = mData;
        connectivity = conn;
    }

    inline int ParallelMesh::numberOfTets() const {
        return int(connectivity->tets.size() / 4);
    }

    inline const int* ParallelMesh::getTet(int id) const {
        return &(connectivity->tets[4*id]);
    }

    inline int ParallelMesh::numberOfPyramids() const {
        return int(connectivity->pyramids.size() / 5);
    }

    inline const int *ParallelMesh::getPyramid(int id) const {
        return &(connectivity->pyramids[5*id]);
    }

    inline int ParallelMesh::numberOfPrisms() const {
        return int(connectivity->prisms.size() / 6);
    }

    inline const int* ParallelMesh::getPrism(int id) const {
        return &(connectivity->prisms[6*id]);
    }

    inline int ParallelMesh::numberOfHexes() const {
        return int(connectivity->hexs.size() / 8);
    }

    inline const int *ParallelMesh::getHex(int id) const {
        return &(connectivity->hexs[8*id]);
    }
}