#include <algorithm>
#include <RangeLoop.h>

#ifdef PARFAIT_WITH_MPI

namespace Parfait {
  inline NodeBasedRedistributor::NodeBasedRedistributor(std::shared_ptr<ParallelMesh> mesh_in,
                                                        vector<int> &part_in)
          : mesh(mesh_in),
            part(part_in)
  { }


  inline std::shared_ptr<ParallelMesh> NodeBasedRedistributor::redistribute() {

      int nproc = MessagePasser::NumberOfProcesses();
      nodeMap.assign(nproc, 0);
      MessagePasser::AllGather(mesh->countNodesAtDegree(0), nodeMap);
      nodeMap.insert(nodeMap.begin(), 0);
      for (unsigned int i = 2; i < nodeMap.size(); i++)
          nodeMap[i] += nodeMap[i - 1];

      auto myNonGhostIds = redistributeNodeIds();
      //for(auto x:myNonGhostIds)
      //    printf("Rank %i: %i\n",MessagePasser::Rank(),x);
      auto recvTets = redistributeCells(myNonGhostIds, mesh->connectivity->tets, 4);
      auto recvPyramids = redistributeCells(myNonGhostIds,mesh->connectivity->pyramids,5);
      auto recvPrisms = redistributeCells(myNonGhostIds,mesh->connectivity->prisms,6);
      auto recvHexs = redistributeCells(myNonGhostIds,mesh->connectivity->hexes,8);


      auto myGhostIds = identifyGhostNodes(myNonGhostIds, recvTets, recvPyramids, recvPrisms, recvHexs);
      auto my_all_node_ids = myNonGhostIds;
      my_all_node_ids.insert(my_all_node_ids.end(),myGhostIds.begin(),myGhostIds.end());
      std::sort(my_all_node_ids.begin(),my_all_node_ids.end());
      auto recvTriangles = redistributeCells(my_all_node_ids,mesh->connectivity->triangles,3);
      auto recvTriangleTags = redistributeTags(my_all_node_ids, mesh->connectivity->triangles,
                                               mesh->metaData->triangleTags, 3);
      auto recvQuads  = redistributeCells(my_all_node_ids,mesh->connectivity->quads,4);
      auto recvQuadTags = redistributeTags(my_all_node_ids, mesh->connectivity->quads,
                                           mesh->metaData->quadTags, 4);
      //for(int i=0;i<recvTriangles.size()/3;++i){
      //    long *p = &recvTriangles[3*i];
      //    printf("Rank %i: triangles %i (%i %i %i)\n",MessagePasser::Rank(),i,p[0],p[1],p[2]);
      //}
      redistributeNodeMetaData(myNonGhostIds,myGhostIds);

      int numberOfNonGhosts = myNonGhostIds.size();
      int numberOfGhosts = myGhostIds.size();

      std::vector<int> ownership_degree(numberOfGhosts+numberOfNonGhosts, 0);

      std::fill(ownership_degree.begin() + myNonGhostIds.size(), ownership_degree.end(), 1);

      std::map<long, int> global_to_local;
      for (int i = 0; i < numberOfGhosts; i++)
          global_to_local[myGhostIds[i]] = i+numberOfNonGhosts;
      for (int i = 0; i < numberOfNonGhosts; i++)
          global_to_local[myNonGhostIds[i]] = i;

      mesh->connectivity->triangles = convertToLocalIds(global_to_local, recvTriangles);
      mesh->connectivity->quads = convertToLocalIds(global_to_local, recvQuads);
      mesh->connectivity->tets = convertToLocalIds(global_to_local, recvTets);
      mesh->connectivity->pyramids = convertToLocalIds(global_to_local, recvPyramids);
      mesh->connectivity->prisms = convertToLocalIds(global_to_local, recvPrisms);
      mesh->connectivity->hexes = convertToLocalIds(global_to_local, recvHexs);
      mesh->metaData->xyz = recvXYZ;
      mesh->metaData->nodeComponentIds = recvAssociatedComponentIds;
      mesh->metaData->triangleTags = recvTriangleTags;
      mesh->metaData->quadTags = recvQuadTags;
      mesh->metaData->globalNodeIds = myNonGhostIds;
      mesh->metaData->globalNodeIds.insert(mesh->metaData->globalNodeIds.end(),myGhostIds.begin(),myGhostIds.end());
      mesh->metaData->nodeOwnershipDegree = ownership_degree;

      return mesh;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributeNodeIds() {
    std::vector<long> recvNodeIds;
      for (int proc:range(MessagePasser::NumberOfProcesses())) {
          int count = 0;
          for (int owner:part)
              if (owner == proc)
                  count++;
          vector<long> sendIds;
          vector<double> sendNodes;
          sendIds.reserve(count);
          sendNodes.reserve(3 * count);
          for (unsigned int localNodeId = 0; localNodeId < part.size(); localNodeId++) {
              if (part[localNodeId] == proc) {
                  auto globalNodeId = mesh->metaData->globalNodeIds[localNodeId];
                  sendIds.push_back(globalNodeId);
              }
          }
          MessagePasser::Gatherv(sendIds, recvNodeIds, proc);
      }
      std::sort(recvNodeIds.begin(), recvNodeIds.end());
    return recvNodeIds;
  }

    inline void NodeBasedRedistributor::redistributeNodeMetaData(std::vector<long>&my_non_ghost_ids,
                                                                 std::vector<long>& my_ghost_ids) {
        std::map<long, int> global_to_local;
        for (unsigned int i = 0; i < mesh->metaData->globalNodeIds.size(); i++)
            global_to_local[mesh->metaData->globalNodeIds[i]] = i;
        for (int proc=0;proc<MessagePasser::NumberOfProcesses();++proc) {
            vector<long> idsOfXyxsINeed;
            if (MessagePasser::Rank() == proc) {
                idsOfXyxsINeed = my_non_ghost_ids;
                idsOfXyxsINeed.insert(idsOfXyxsINeed.end(), my_ghost_ids.begin(), my_ghost_ids.end());
            }
            MessagePasser::Broadcast(idsOfXyxsINeed, proc);
            vector<double> sendXYZ;
            vector<long> sendGlobalNodeId;
            vector<int> sendAssociatedComponentId;
            for (auto globalNodeId : idsOfXyxsINeed) {
                if(amItheOwnerOfThisNode(globalNodeId, global_to_local)) {
                    int localNodeId = global_to_local[globalNodeId];
                    //printf("Rank %i owns gid %i (%f %f %f) (local id %i)\n",MessagePasser::Rank(),globalNodeId,mesh->metaData->xyz[3*localNodeId+0],
                    //mesh->metaData->xyz[3*localNodeId+1], mesh->metaData->xyz[3*localNodeId+2],localNodeId);
                    sendXYZ.push_back(mesh->metaData->xyz[3 * localNodeId + 0]);
                    sendXYZ.push_back(mesh->metaData->xyz[3 * localNodeId + 1]);
                    sendXYZ.push_back(mesh->metaData->xyz[3 * localNodeId + 2]);
                    sendGlobalNodeId.push_back(globalNodeId);
                    sendAssociatedComponentId.push_back(mesh->metaData->nodeComponentIds[localNodeId]);
                }
            }

            std::vector<double> gatheredXyz;
            MessagePasser::Gatherv(sendXYZ, gatheredXyz, proc);
            std::vector<long> gatheredIds;
            MessagePasser::Gatherv(sendGlobalNodeId, gatheredIds, proc);
            std::vector<int> gatheredComponentIds;
            MessagePasser::Gatherv(sendAssociatedComponentId, gatheredComponentIds, proc);
            if (MessagePasser::Rank() == proc) {
                recvXYZ.assign(gatheredXyz.size(),0);
                recvAssociatedComponentIds.assign(gatheredComponentIds.size(),-1);
                for (unsigned int index = 0; index < gatheredIds.size(); index++) {
                    long globalNodeId = gatheredIds[index];
                    int localId = getLocalNodeId(globalNodeId, my_non_ghost_ids,my_ghost_ids);
                    for (int i = 0; i < 3; i++)
                        recvXYZ[3 * localId + i] = gatheredXyz[3 * index + i];
                    recvAssociatedComponentIds[localId] = gatheredComponentIds[index];
                }
            }
        }
    }

    inline bool NodeBasedRedistributor::amItheOwnerOfThisNode(long globalId, const std::map<long, int>& globalToLocal) {
        auto it = globalToLocal.find(globalId);
        if(globalToLocal.end() == it)
            return false;
        int localId = it->second;
        if(mesh->metaData->nodeOwnershipDegree[localId] == 0)
            return true;
        return false;
    }
    inline std::vector<int> NodeBasedRedistributor::redistributeTags(std::vector<long> &my_ghost_ids,
                                                                     std::vector<int> &cells,
                                                                     std::vector<int> &tags,
                                                                     int cellSize) {
        std::vector<long> recvCells;
        std::vector<int> recvCellTags;
        for (int proc:range(MessagePasser::NumberOfProcesses())) {
            vector<long> sendLocalCellIds;
            vector<long> neededNodeIds;
            vector<int> sendCellTags;
            if (MessagePasser::Rank() == proc)
                neededNodeIds = my_ghost_ids;
            MessagePasser::Broadcast(neededNodeIds, proc);
            for (unsigned int localCellId = 0; localCellId < cells.size() / cellSize; localCellId++) {
                for (int j=0;j<cellSize;++j) {
                    int localNodeId = cells[cellSize * localCellId + j];
                    auto globalId = mesh->metaData->globalNodeIds[localNodeId];
                    if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalId)) {
                        sendCellTags.push_back(tags[localCellId]);
                        break;
                    }
                }
            }
            MessagePasser::Gatherv(sendCellTags, recvCellTags, proc);
        }
        return recvCellTags;
    }

  inline std::vector<long> NodeBasedRedistributor::redistributeCells(std::vector<long> &my_non_ghost_ids,
                                                                     std::vector<int> &cells, int cellSize) {
      vector<long> sendCellIds;
    vector<long> recvCells;
      sendCellIds.reserve(cells.size());
      for (int proc = 0; proc < MessagePasser::NumberOfProcesses(); proc++) {
          sendCellIds.clear();
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_non_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int i = 0; i < cells.size() / cellSize; i++) {
              if(iShouldSendThisCell(&cells[cellSize*i],cellSize,neededNodeIds))
                      sendCellIds.push_back(i);
          }
          vector<long> sendCells;
          sendCells.reserve(sendCellIds.size());
          for (auto id:sendCellIds)
              for (int j:range(cellSize))
                  sendCells.push_back(mesh->metaData->globalNodeIds[cells[cellSize * id + j]]);
          MessagePasser::Gatherv(sendCells, recvCells, proc);
      }
    return recvCells;
  }

    inline bool NodeBasedRedistributor::iShouldSendThisCell(int* cell,int cellSize,const std::vector<long>& neededNodeIds){
        if(mesh->metaData->nodeOwnershipDegree[cell[0]] != 0) return false;
        for(int i=0;i<cellSize;i++){
            int localNodeId = cell[i];
            long globalNodeId = mesh->metaData->globalNodeIds[localNodeId];
            if(not std::binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalNodeId))
                return false;
        }
        return true;
    }

  inline std::vector<long> NodeBasedRedistributor::identifyGhostNodes(std::vector<long>&my_non_ghost_ids,
        std::vector<long>& recvTets,std::vector<long>& recvPyramids,
        std::vector<long>& recvPrisms, std::vector<long>& recvHexs) {
      std::set<long> uniqueGhostNodeIds;
      if (!std::is_sorted(my_non_ghost_ids.begin(), my_non_ghost_ids.end()))
          throw std::logic_error("Recv node Ids expected in order.");
      for (auto id:recvTets)
          if (!std::binary_search(my_non_ghost_ids.begin(), my_non_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);
      for (auto id:recvPyramids)
          if (!std::binary_search(my_non_ghost_ids.begin(), my_non_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);
      for (auto id:recvPrisms)
          if (!std::binary_search(my_non_ghost_ids.begin(), my_non_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);
      for (auto id:recvHexs)
          if (!std::binary_search(my_non_ghost_ids.begin(), my_non_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);

      return std::vector<long>(uniqueGhostNodeIds.begin(), uniqueGhostNodeIds.end());
  }

  inline std::vector<int> NodeBasedRedistributor::convertToLocalIds(std::map<long, int> global_to_local_map,
                                                                    const std::vector<long> &ids) const {
      std::vector<int> local_ids;
      for (auto id:ids)
          local_ids.push_back(global_to_local_map[id]);
      return local_ids;
  }

  inline int NodeBasedRedistributor::getLocalNodeId(long globalNodeId,std::vector<long>&my_non_ghost_ids,
  std::vector<long>& my_ghost_ids) {
      int numberOfStuffs = my_non_ghost_ids.size();
      if(std::binary_search(my_non_ghost_ids.begin(),my_non_ghost_ids.end(),globalNodeId))
          return std::distance(my_non_ghost_ids.begin(),std::lower_bound(my_non_ghost_ids.begin(),my_non_ghost_ids.end(),globalNodeId));
      else if(std::binary_search(my_ghost_ids.begin(),my_ghost_ids.end(),globalNodeId))
          return std::distance(my_ghost_ids.begin(),std::lower_bound(my_ghost_ids.begin(),my_ghost_ids.end(),globalNodeId))+numberOfStuffs;
      else
          throw std::domain_error("Didn't find the id...");
  }
}
#endif