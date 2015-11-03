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

      nproc = MessagePasser::NumberOfProcesses();
      nodeMap.assign(nproc, 0);
      MessagePasser::AllGather(mesh->countNodesAtDegree(0), nodeMap);
      nodeMap.insert(nodeMap.begin(), 0);
      for (unsigned int i = 2; i < nodeMap.size(); i++)
          nodeMap[i] += nodeMap[i - 1];

      auto myNonGhostIds = redistributeNodeIds();
      auto recvTriangles = redistributeTriangles(myNonGhostIds);
      auto recvQuads  = redistributeQuads(myNonGhostIds);
      auto recvTets = redistributeTets(myNonGhostIds);
      auto recvPyramids = redistributePyramids(myNonGhostIds);
      auto recvPrisms = redistributePrisms(myNonGhostIds);
      auto recvHexs = redistributeHexes(myNonGhostIds);

      myGhostIds = identifyGhostNodes(myNonGhostIds, recvTets, recvPyramids, recvPrisms, recvHexs);
    concatinateGhostAndNonGhostIds(myNonGhostIds, myGhostIds);
      auto my_all_ids = myNonGhostIds;
    my_all_ids.insert(my_all_ids.end(),myGhostIds.begin(),myGhostIds.end());
      redistributeNodeMetaData(my_all_ids,myNonGhostIds);

      std::vector<int> ownership_degree(myAllIds.size(), 0);

      std::fill(ownership_degree.begin() + myNonGhostIds.size(), ownership_degree.end(), 1);

      std::map<long, int> global_to_local;
      for (unsigned int i = 0; i < myAllIds.size(); i++)
          global_to_local.insert(std::make_pair(myAllIds[i], i));

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
      mesh->metaData->globalNodeIds = myAllIds;
      mesh->metaData->nodeOwnershipDegree = ownership_degree;
      return mesh;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributeNodeIds() {
    std::vector<long> recvNodeIds;
      for (int proc:range(nproc)) {
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

  inline void NodeBasedRedistributor::redistributeNodeMetaData(std::vector<long>& my_all_ids,std::vector<long>& my_ghost_ids) {
      std::map<long, int> global_to_local;
      for (unsigned int i = 0; i < mesh->metaData->globalNodeIds.size(); i++)
          global_to_local.insert(std::make_pair(mesh->metaData->globalNodeIds[i], i));
      for (int proc:range(nproc)) {
          vector<long> neededXYZ;
          if (MessagePasser::Rank() == proc)
              neededXYZ = my_all_ids;
          MessagePasser::Broadcast(neededXYZ, proc);
          vector<double> sendXYZ;
          vector<long> sendGlobalNodeId;
          vector<int> sendAssociatedComponentId;
          for (auto globalNodeId : neededXYZ) {
              auto it = global_to_local.find(globalNodeId);
              if (not (it == global_to_local.end())) {
                  auto localNodeId = it->second;
                  if(mesh->metaData->nodeOwnershipDegree[localNodeId] == 0) {
                      sendXYZ.push_back(mesh->metaData->xyz[3 * localNodeId + 0]);
                      sendXYZ.push_back(mesh->metaData->xyz[3 * localNodeId + 1]);
                      sendXYZ.push_back(mesh->metaData->xyz[3 * localNodeId + 2]);
                      sendGlobalNodeId.push_back(globalNodeId);
                      sendAssociatedComponentId.push_back(mesh->metaData->nodeComponentIds[localNodeId]);
                  }
              }
          }

          std::vector<double> just_recv_xyz;
          MessagePasser::Gatherv(sendXYZ, just_recv_xyz, proc);
          std::vector<long> just_recv_xyz_global_node_ids;
          MessagePasser::Gatherv(sendGlobalNodeId, just_recv_xyz_global_node_ids, proc);
          std::vector<int> just_recv_associated_component_ids;
          MessagePasser::Gatherv(sendAssociatedComponentId, just_recv_associated_component_ids, proc);
          if (MessagePasser::Rank() == proc) {
              recvXYZ.resize(3 * just_recv_xyz_global_node_ids.size());
              recvAssociatedComponentIds.resize(just_recv_associated_component_ids.size());
              for (unsigned int index = 0; index < just_recv_xyz_global_node_ids.size(); index++) {
                  auto globalNodeId = just_recv_xyz_global_node_ids[index];
                  int localId = getLocalNodeId(globalNodeId,my_ghost_ids);
                  for (int i = 0; i < 3; i++)
                      recvXYZ[3 * localId + i] = just_recv_xyz[3 * index + i];
                  recvAssociatedComponentIds[localId] = just_recv_associated_component_ids[index];
              }
          }
      }
  }

  inline std::vector<long> NodeBasedRedistributor::redistributeTriangles(std::vector<long>& my_ghost_ids) {
    std::vector<long> recvTriangles;
      for (int proc:range(nproc)) {
          vector<long> sendTriangleIds;
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int localCellId = 0; localCellId < mesh->connectivity->triangles.size() / 3; localCellId++) {
              for (int j:range(3)) {
                  int localNodeId = mesh->connectivity->triangles[3 * localCellId + j];
                  auto globalId = mesh->metaData->globalNodeIds[localNodeId];
                  if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalId)) {
                      sendTriangleIds.push_back(localCellId);
                      break;
                  }
              }
          }
          vector<long> sendTriangles;
          vector<int> sendTriangleTags;
          for (auto &localCellId:sendTriangleIds) {
              sendTriangleTags.push_back(mesh->metaData->triangleTags[localCellId]);
              for (int j:range(3)) {
                  int localNodeId = mesh->connectivity->triangles[3 * localCellId + j];
                  auto globalId = mesh->metaData->globalNodeIds[localNodeId];
                  sendTriangles.push_back(globalId);
              }
          }
          vector<int> tmpMap;
          MessagePasser::Gatherv(sendTriangles, recvTriangles, tmpMap, proc);
          MessagePasser::Gatherv(sendTriangleTags, recvTriangleTags, tmpMap, proc);
      }
    return recvTriangles;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributeQuads(std::vector<long>& my_ghost_ids) {
    std::vector<long> recvQuads;
      for (int proc:range(nproc)) {
          vector<long> sendQuadIds;
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int localCellId = 0; localCellId < mesh->connectivity->quads.size() / 4; localCellId++) {
              for (int j:range(4)) {
                  int localNodeId = mesh->connectivity->quads[4 * localCellId + j];
                  auto globalNodeId = mesh->metaData->globalNodeIds[localNodeId];
                  if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalNodeId)) {
                      sendQuadIds.push_back(localCellId);
                      break;
                  }
              }
          }
          vector<long> sendQuads;
          vector<int> sendQuadTags;
          for (const auto &localCellId :sendQuadIds) {
              sendQuadTags.push_back(mesh->metaData->quadTags[localCellId]);
              for (int j:range(4)) {
                  int localNodeId = mesh->connectivity->quads[4 * localCellId + j];
                  auto globalNodeId = mesh->metaData->globalNodeIds[localNodeId];
                  sendQuads.push_back(globalNodeId);
              }
          }
          MessagePasser::Gatherv(sendQuads, recvQuads, proc);
          MessagePasser::Gatherv(sendQuadTags, recvQuadTags, proc);
      }
    return recvQuads;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributeTets(std::vector<long>& my_ghost_ids) {
      vector<long> sendTetIds;
    vector<long> recvTets;
      sendTetIds.reserve(4 * mesh->connectivity->tets.size());
      for (int proc = 0; proc < nproc; proc++) {
          sendTetIds.clear();
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int i = 0; i < mesh->connectivity->tets.size() / 4; i++) {
              for (int j = 0; j < 4; j++) {
                  long globalId = mesh->metaData->globalNodeIds[mesh->connectivity->tets[4 * i + j]];
                  if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalId)) {
                      sendTetIds.push_back(i);
                      break;
                  }
              }
          }
          vector<long> sendTets;
          sendTets.reserve(4 * sendTetIds.size());
          for (auto id:sendTetIds)
              for (int j:range(4))
                  sendTets.push_back(mesh->metaData->globalNodeIds[mesh->connectivity->tets[4 * id + j]]);
          MessagePasser::Gatherv(sendTets, recvTets, proc);
      }
    return recvTets;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributePyramids(std::vector<long>& my_ghost_ids) {
    std::vector<long> sendPyramidIds;
    std::vector<long> recvPyramids;
      sendPyramidIds.reserve(5 * mesh->connectivity->pyramids.size());
      for (int proc:range(nproc)) {
          sendPyramidIds.clear();
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int i = 0; i < mesh->connectivity->pyramids.size() / 5; i++) {
              for (int j:range(5)) {
                  long globalId = mesh->metaData->globalNodeIds[mesh->connectivity->pyramids[5 * i + j]];
                  if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalId)) {
                      sendPyramidIds.push_back(i);
                      break;
                  }
              }
          }
          vector<long> sendPyramids;
          sendPyramids.reserve(5 * sendPyramidIds.size());
          for (auto id:sendPyramidIds)
              for (int j:range(5))
                  sendPyramids.push_back(mesh->metaData->globalNodeIds[mesh->connectivity->pyramids[5 * id + j]]);
          MessagePasser::Gatherv(sendPyramids, recvPyramids, proc);
      }
    return recvPyramids;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributePrisms(std::vector<long>& my_ghost_ids) {
    std::vector<long> sendPrismIds;
    std::vector<long> recvPrisms;
      sendPrismIds.reserve(6 * mesh->connectivity->prisms.size());
      for (int proc:range(nproc)) {
          sendPrismIds.clear();
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int i = 0; i < mesh->connectivity->prisms.size() / 6; i++) {
              for (int j:range(6)) {
                  long globalId = mesh->metaData->globalNodeIds[mesh->connectivity->prisms[6 * i + j]];
                  if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalId)) {
                      sendPrismIds.push_back(i);
                      break;
                  }
              }
          }
          vector<long> sendPrisms;
          sendPrisms.reserve(6 * sendPrismIds.size());
          for (auto id:sendPrismIds)
              for (int j:range(6))
                  sendPrisms.push_back(mesh->metaData->globalNodeIds[mesh->connectivity->prisms[6 * id + j]]);
          MessagePasser::Gatherv(sendPrisms, recvPrisms, proc);
      }
    return recvPrisms;
  }

  inline std::vector<long> NodeBasedRedistributor::redistributeHexes(std::vector<long>& my_ghost_ids) {
      vector<long> sendHexIds;
    std::vector<long> recvHexs;
      sendHexIds.reserve(8 * mesh->connectivity->hexes.size());
      for (int proc:range(nproc)) {
          sendHexIds.clear();
          vector<long> neededNodeIds;
          if (MessagePasser::Rank() == proc)
              neededNodeIds = my_ghost_ids;
          MessagePasser::Broadcast(neededNodeIds, proc);
          for (unsigned int i = 0; i < mesh->connectivity->hexes.size() / 8; i++) {
              for (int j:range(8)) {
                  long globalId = mesh->metaData->globalNodeIds[mesh->connectivity->hexes[8 * i + j]];
                  if (binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalId)) {
                      sendHexIds.push_back(i);
                      break;
                  }
              }
          }
          vector<long> sendHexs;
          sendHexs.reserve(8 * sendHexIds.size());
          for (auto id:sendHexIds)
              for (int j:range(8))
                  sendHexs.push_back(mesh->metaData->globalNodeIds[mesh->connectivity->hexes[8 * id + j]]);
          MessagePasser::Gatherv(sendHexs, recvHexs, proc);
      }
    return recvHexs;
  }

  inline std::vector<long> NodeBasedRedistributor::identifyGhostNodes(std::vector<long>& my_ghost_ids,
        std::vector<long>& recvTets,std::vector<long>& recvPyramids,
        std::vector<long>& recvPrisms, std::vector<long>& recvHexs) {
      std::set<long> uniqueGhostNodeIds;
      if (!std::is_sorted(my_ghost_ids.begin(), my_ghost_ids.end()))
          throw std::logic_error("Recv node Ids expected in order.");
      for (auto id:recvTets)
          if (!std::binary_search(my_ghost_ids.begin(), my_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);
      for (auto id:recvPyramids)
          if (!std::binary_search(my_ghost_ids.begin(), my_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);
      for (auto id:recvPrisms)
          if (!std::binary_search(my_ghost_ids.begin(), my_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);
      for (auto id:recvHexs)
          if (!std::binary_search(my_ghost_ids.begin(), my_ghost_ids.end(), id))
              uniqueGhostNodeIds.insert(id);

      return std::vector<long>(uniqueGhostNodeIds.begin(), uniqueGhostNodeIds.end());
  }

  inline void NodeBasedRedistributor::concatinateGhostAndNonGhostIds(std::vector<long> &my_non_ghost_ids,
                                                                     std::vector<long> &my_ghost_ids) {
      myAllIds = my_non_ghost_ids;
      myAllIds.insert(myAllIds.end(), my_ghost_ids.begin(), my_ghost_ids.end());
  }

  inline std::vector<int> NodeBasedRedistributor::convertToLocalIds(std::map<long, int> global_to_local_map,
                                                                    const std::vector<long> &ids) {
      std::vector<int> local_ids;
      for (auto id:ids)
          local_ids.push_back(global_to_local_map[id]);
      return local_ids;
  }

  inline int NodeBasedRedistributor::getLocalNodeId(long globalNodeId,std::vector<long>& my_ghost_ids) {
      auto it = std::lower_bound(my_ghost_ids.begin(), my_ghost_ids.end(), globalNodeId);
      if (it == my_ghost_ids.end()) {
          it = std::lower_bound(myGhostIds.begin(), myGhostIds.end(), globalNodeId);
          if (it != myGhostIds.end())
              return std::distance(myGhostIds.begin(), it) + my_ghost_ids.size();
          else
              throw std::logic_error("saldfjsdf");
      }
      return std::distance(my_ghost_ids.begin(), it);
  }
}
#endif