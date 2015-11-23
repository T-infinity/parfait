#include <algorithm>
#include <RangeLoop.h>
#include "timing.h"

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

      auto beginning = Clock::now();
      auto myNonGhostIds = redistributeNodeIds();
      auto afterNodes = Clock::now();
      auto recvTets = redistributeCells(myNonGhostIds, mesh->connectivity->tets, 4);
      auto afterTets = Clock::now();
      auto recvPyramids = redistributeCells(myNonGhostIds,mesh->connectivity->pyramids,5);
      auto afterPyramids = Clock::now();
      auto recvPrisms = redistributeCells(myNonGhostIds,mesh->connectivity->prisms,6);
      auto afterPrisms = Clock::now();
      auto recvHexs = redistributeCells(myNonGhostIds,mesh->connectivity->hexes,8);
      auto afterHexs = Clock::now();


      auto myGhostIds = identifyGhostNodes(myNonGhostIds, recvTets, recvPyramids, recvPrisms, recvHexs);
      auto afterIdentifyingGhosts = Clock::now();
      auto my_all_node_ids = myNonGhostIds;
      my_all_node_ids.insert(my_all_node_ids.end(),myGhostIds.begin(),myGhostIds.end());
      std::sort(my_all_node_ids.begin(),my_all_node_ids.end());
      auto recvTriangles = redistributeSurfaceCells(my_all_node_ids,mesh->connectivity->triangles,3);
      auto afterTriangles = Clock::now();
      auto recvTriangleTags = redistributeTags(my_all_node_ids, mesh->connectivity->triangles,
                                               mesh->metaData->triangleTags, 3);
      auto afterTriangleTags = Clock::now();
      auto recvQuads  = redistributeSurfaceCells(my_all_node_ids,mesh->connectivity->quads,4);
      auto afterQuads = Clock::now();
      auto recvQuadTags = redistributeTags(my_all_node_ids, mesh->connectivity->quads,
                                           mesh->metaData->quadTags, 4);
      auto afterQuadTags = Clock::now();
      redistributeNodeMetaData(myNonGhostIds,myGhostIds);
      auto afterMetaData = Clock::now();

      int numberOfNonGhosts = myNonGhostIds.size();
      int numberOfGhosts = myGhostIds.size();

      std::vector<int> ownership_degree(numberOfGhosts+numberOfNonGhosts, 0);

      std::fill(ownership_degree.begin() + myNonGhostIds.size(), ownership_degree.end(), 1);

      std::map<long, int> global_to_local;
      for (int i = 0; i < numberOfGhosts; i++)
          global_to_local[myGhostIds[i]] = i+numberOfNonGhosts;
      for (int i = 0; i < numberOfNonGhosts; i++)
          global_to_local[myNonGhostIds[i]] = i;

      auto afterGhostStuff = Clock::now();
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
      auto afterFilling = Clock::now();

      if(MessagePasser::Rank() == 0) {
          printf("Time to redistribute node ids\n");
          printReadableElapsedTime(beginning,afterNodes);
          printf("Time to redistribute tets\n");
          printReadableElapsedTime(afterNodes,afterTets);
          printf("Time to redistribute pyramids\n");
          printReadableElapsedTime(afterTets,afterPyramids);
          printf("Time to redistribute prisms\n");
          printReadableElapsedTime(afterPyramids,afterPrisms);
          printf("Time to redistribute hexs\n");
          printReadableElapsedTime(afterPrisms,afterHexs);
          printf("Time to identify ghosts\n");
          printReadableElapsedTime(afterHexs,afterIdentifyingGhosts);
          printf("Time to redistribute triangles\n");
          printReadableElapsedTime(afterIdentifyingGhosts,afterTriangles);
          printf("Time to redistribute triangle tags\n");
          printReadableElapsedTime(afterTriangles,afterTriangleTags);
          printf("Time to redistribute quads\n");
          printReadableElapsedTime(afterTriangleTags,afterQuads);
          printf("Time to redistribute quad tags\n");
          printReadableElapsedTime(afterQuads,afterQuadTags);
          printf("Time to redistribute node meta data\n");
          printReadableElapsedTime(afterQuadTags,afterMetaData);
          printf("Time to do ghost stuff\n");
          printReadableElapsedTime(afterMetaData,afterGhostStuff);
          printf("Time to fill mesh vectors\n");
          printReadableElapsedTime(afterGhostStuff,afterFilling);
      }

      return mesh;
  }

    inline std::map<long,std::vector<int>> NodeBasedRedistributor::mapNodesToCells(std::vector<long>& globalNodeIds,
                                                                                   std::vector<int>& nodeOwnershipDegree,
                                                                                 std::vector<int>& cells,
                                                                                 int cellSize){
        std::map<long,std::vector<int>> nodeToCell;
        int ncells = (int)cells.size()/cellSize;
        for(int i=0;i<ncells;++i){
            for(int j=0;j<cellSize;++j){
                if(0 == nodeOwnershipDegree[cells[cellSize*i+j]]) {
                    long nodeId = globalNodeIds[cells[cellSize * i + j]];
                    auto iter = nodeToCell.find(nodeId);
                    if (iter != nodeToCell.end())
                        iter->second.push_back(i);
                    else
                        nodeToCell[nodeId] = {i};
                }
            }
        }
        return nodeToCell;
    }

  inline std::vector<long> NodeBasedRedistributor::redistributeNodeIds() {
      if(MessagePasser::Rank() == 0) printf("Redistributing node ids\n");
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
        if(MessagePasser::Rank() == 0) printf("Redistributing node meta data\n");
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
    inline std::vector<int> NodeBasedRedistributor::redistributeTags(std::vector<long> &my_non_ghost_ids,
                                                                     std::vector<int> &cells,
                                                                     std::vector<int> &tags,
                                                                     int cellSize) {
        if(MessagePasser::Rank() == 0) printf("Redistributing boundary tags (%i)\n",cellSize);
        std::vector<int> recvCellTags;
        for (int proc:range(MessagePasser::NumberOfProcesses())) {
            vector<long> neededNodeIds;
            vector<int> sendCellTags;
            if (MessagePasser::Rank() == proc)
                neededNodeIds = my_non_ghost_ids;
            MessagePasser::Broadcast(neededNodeIds, proc);
            for (unsigned int i = 0; i < cells.size() / cellSize; i++) {
                if(iShouldSendThisSurfaceCell(&cells[cellSize*i],cellSize,neededNodeIds))
                    sendCellTags.push_back(tags[i]);
            }
            MessagePasser::Gatherv(sendCellTags, recvCellTags, proc);
        }
        return recvCellTags;
    }

    inline std::vector<long> NodeBasedRedistributor::redistributeCells(std::vector<long> &my_non_ghost_ids,
                                                                       std::vector<int> &cells, int cellSize) {
        if(MessagePasser::Rank() == 0) printf("Redistributing cells (%i)\n",cellSize);
        vector<long> recvCells;

        auto nodeToCell = mapNodesToCells(mesh->metaData->globalNodeIds,mesh->metaData->nodeOwnershipDegree,cells,cellSize);

        for (int proc = 0; proc < MessagePasser::NumberOfProcesses(); proc++) {
            vector<long> neededNodeIds;
            vector<long> sendCellIds;
            std::vector<bool> cellHasBeenAdded(cells.size(),false);
            if (MessagePasser::Rank() == proc)
                neededNodeIds = my_non_ghost_ids;
            MessagePasser::Broadcast(neededNodeIds, proc);
            for(long nodeId:neededNodeIds){
                auto iter = nodeToCell.find(nodeId);
                if(iter != nodeToCell.end()) {
                    for (int cellId:iter->second) {
                        if (not cellHasBeenAdded[cellId]) {
                           sendCellIds.push_back(cellId);
                            cellHasBeenAdded[cellId] = true;
                        }
                    }
                }
            }
            //for (unsigned int i = 0; i < cells.size() / cellSize; i++) {
            //    if(iShouldSendThisCell(&cells[cellSize*i],cellSize,neededNodeIds))
            //        sendCellIds.push_back(i);
           // }
            vector<long> sendCells;
            sendCells.reserve(cellSize*sendCellIds.size());
            for (auto id:sendCellIds)
                for (int j:range(cellSize))
                    sendCells.push_back(mesh->metaData->globalNodeIds[cells[cellSize * id + j]]);
            MessagePasser::Gatherv(sendCells, recvCells, proc);
        }
        return recvCells;
    }

    inline std::vector<long> NodeBasedRedistributor::redistributeSurfaceCells(std::vector<long> &my_non_ghost_ids,
                                                                       std::vector<int> &cells, int cellSize) {
        if(MessagePasser::Rank() == 0) printf("Redistributing surface cells (%i)\n",cellSize);
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
                if(iShouldSendThisSurfaceCell(&cells[cellSize*i],cellSize,neededNodeIds))
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
            if(std::binary_search(neededNodeIds.begin(),neededNodeIds.end(),globalNodeId))
                return true;
        }
        return false;
    }

    inline bool NodeBasedRedistributor::iShouldSendThisSurfaceCell(int* cell,int cellSize,const std::vector<long>& neededNodeIds){
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
      if(MessagePasser::Rank() == 0) printf("Identifying ghost nodes\n");
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