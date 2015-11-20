#ifndef PARALLEL_MESH_REDISTRIBUTOR_H
#define PARALLEL_MESH_REDISTRIBUTOR_H

#ifdef PARFAIT_WITH_MPI

#include <vector>
#include <MessagePasser.h>
#include <ParallelImportedUgrid.h>
#include "ParallelMesh.h"

namespace Parfait {
  class NodeBasedRedistributor {
      template<class T> using vector = std::vector<T>;
  public:
      NodeBasedRedistributor(std::shared_ptr<ParallelMesh> mesh, std::vector<int> &part);

      std::shared_ptr<ParallelMesh> redistribute();
  private:

      std::vector<long> redistributeNodeIds();
      std::vector<int> redistributeTags(std::vector<long> &my_non_ghost_ids,
                                        std::vector<int> &cells, std::vector<int> &tags, int cellSize);
      std::vector<long> redistributeCells(std::vector<long> &my_non_ghost_ids,
                                          std::vector<int> &cells, int cellSize,
                                          std::map<long,std::vector<int>>& nodeToCell);
      std::vector<long> redistributeSurfaceCells(std::vector<long> &my_non_ghost_ids,
                                          std::vector<int> &cells, int cellSize);

      std::vector<long> identifyGhostNodes(std::vector<long>&my_non_ghost_ids,
                                           std::vector<long>& recvTets,
                                           std::vector<long>& recvPyramids,
                                           std::vector<long>& recvPrisms,
                                           std::vector<long>& recvHexs);
      void redistributeNodeMetaData(std::vector<long>&my_non_ghost_ids,
                                    std::vector<long>& my_ghost_ids);
      std::shared_ptr<ParallelMesh> mesh;
      std::vector<int> part;
      std::vector<int> nodeMap;
      std::vector<int> recvAssociatedComponentIds;
      std::vector<double> recvXYZ;


      int getLocalNodeId(long globalNodeId,std::vector<long>&my_non_ghost_ids,
                         std::vector<long>& my_ghost_ids);


      std::vector<int> convertToLocalIds(std::map<long,int> global_to_local_map,const std::vector<long>& ids) const;
      bool amItheOwnerOfThisNode(long globalId, const std::map<long, int>& globalToLocal);
      bool iShouldSendThisCell(int* cell, int cellSize,const std::vector<long>& neededNodeIds);
      bool iShouldSendThisSurfaceCell(int* cell, int cellSize,const std::vector<long>& neededNodeIds);
      std::map<long,std::vector<int>> mapNodesToCells(std::vector<long>& globalNodeIds,
                                                    std::vector<int>& cells,int cellSize);
  };


}
#include "NodeCenteredRedistributor.hpp"

#endif
#endif
