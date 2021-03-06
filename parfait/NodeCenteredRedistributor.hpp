
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include <algorithm>
#include <map>
#include "Timing.h"
#include "ParallelMeshReader.h"

namespace Parfait {
    inline NodeBasedRedistributor::NodeBasedRedistributor(
            MessagePasser mp,
            std::shared_ptr<ParallelMesh> mesh_in,
            std::vector<int>& part_in)
            : mp(mp),
              mesh(std::make_shared<ParallelMeshBuilder>(mp, *mesh_in.get())),
              mesh_old(mesh_in),
              part(part_in) {}

    inline std::shared_ptr<ParallelMesh> NodeBasedRedistributor::redistribute() {

        int nproc = mp.NumberOfProcesses();
        nodeMap.assign(nproc, 0);
        int num_owned = 0;
        for (unsigned int n = 0; n < mesh->data->xyz.size() / 3; n++) {
            if (mesh->data->nodeOwnerRank[n] == mp.Rank())
                num_owned++;
        }
        mp.AllGather(num_owned, nodeMap);
        nodeMap.insert(nodeMap.begin(), 0);
        for (unsigned int i = 2; i < nodeMap.size(); i++)
            nodeMap[i] += nodeMap[i - 1];

        auto beginning = Clock::now();
        auto myNonGhostIds = redistributeNodeIds();
        auto afterNodes = Clock::now();
        auto recvTets = redistributeCells(myNonGhostIds, mesh->data->tets, 4);
        auto afterTets = Clock::now();
        auto recvPyramids = redistributeCells(myNonGhostIds, mesh->data->pyramids, 5);
        auto afterPyramids = Clock::now();
        auto recvPrisms = redistributeCells(myNonGhostIds, mesh->data->prisms, 6);
        auto afterPrisms = Clock::now();
        auto recvHexs = redistributeCells(myNonGhostIds, mesh->data->hexs, 8);
        auto afterHexs = Clock::now();

        auto myGhostIds = identifyGhostNodes(myNonGhostIds, recvTets, recvPyramids, recvPrisms, recvHexs);
        auto afterIdentifyingGhosts = Clock::now();

        auto recvTriangles = redistributeSurfaceCells(myNonGhostIds, mesh->data->triangles, 3);
        auto afterTriangles = Clock::now();
        auto recvTriangleTags = redistributeTags(myNonGhostIds, mesh->data->triangles,
                                                 mesh->data->triangleTags, 3);
        auto afterTriangleTags = Clock::now();
        auto recvQuads = redistributeSurfaceCells(myNonGhostIds, mesh->data->quads, 4);
        auto afterQuads = Clock::now();

        auto recvQuadTags = redistributeTags(myNonGhostIds, mesh->data->quads,
                                             mesh->data->quadTags, 4);
        auto afterQuadTags = Clock::now();
        redistributeNodeMetaData(myNonGhostIds, myGhostIds);
        auto afterMetaData = Clock::now();

        int numberOfNonGhosts = myNonGhostIds.size();
        int numberOfGhosts = myGhostIds.size();

        std::vector<int> ownership_degree(numberOfGhosts + numberOfNonGhosts, 0);

        std::fill(ownership_degree.begin() + myNonGhostIds.size(), ownership_degree.end(), 1);

        std::map<long, int> global_to_local;
        for (int i = 0; i < numberOfGhosts; i++)
            global_to_local[myGhostIds[i]] = i + numberOfNonGhosts;
        for (int i = 0; i < numberOfNonGhosts; i++)
            global_to_local[myNonGhostIds[i]] = i;

        auto afterGhostStuff = Clock::now();
        mesh->data->triangles = convertToLocalIds(global_to_local, recvTriangles);
        mesh->data->quads = convertToLocalIds(global_to_local, recvQuads);
        mesh->data->tets = convertToLocalIds(global_to_local, recvTets);
        mesh->data->pyramids = convertToLocalIds(global_to_local, recvPyramids);
        mesh->data->prisms = convertToLocalIds(global_to_local, recvPrisms);
        mesh->data->hexs = convertToLocalIds(global_to_local, recvHexs);
        mesh->data->xyz = recvXYZ;
        mesh->data->nodeComponentIds = recvAssociatedComponentIds;
        mesh->data->triangleTags = recvTriangleTags;
        mesh->data->quadTags = recvQuadTags;
        mesh->data->globalNodeIds = myNonGhostIds;
        mesh->data->globalNodeIds.insert(mesh->data->globalNodeIds.end(), myGhostIds.begin(), myGhostIds.end());
        auto afterFilling = Clock::now();

        mesh->data->nodeOwnerRank =
                setNodeOwnerRank(mp, ownership_degree, mesh->data->globalNodeIds);

        return mesh->exportMesh();
    }

    inline std::map<long, std::vector<int>> NodeBasedRedistributor::mapNodesToCells(std::vector<long>& globalNodeIds,
                                                                                    std::vector<int>& cells,
                                                                                    int cellSize) {
        std::map<long, std::vector<int>> nodeToCell;
        int ncells = (int) cells.size() / cellSize;
        for (int i = 0; i < ncells; ++i) {
            for (int j = 0; j < cellSize; ++j) {
                long nodeId = globalNodeIds[cells[cellSize * i + j]];
                auto iter = nodeToCell.find(nodeId);
                if (iter != nodeToCell.end())
                    iter->second.push_back(i);
                else
                    nodeToCell[nodeId] = {i};
            }
        }
        return nodeToCell;
    }

    inline std::vector<long> NodeBasedRedistributor::redistributeNodeIds() {
        std::vector<long> recvNodeIds;
        for (int proc = 0; proc < mp.NumberOfProcesses(); proc++) {
            int count = 0;
            for (int owner:part)
                if (owner == proc)
                    count++;
            std::vector<long> sendIds;
            std::vector<double> sendNodes;
            sendIds.reserve(count);
            sendNodes.reserve(3 * count);
            for (unsigned int localNodeId = 0; localNodeId < part.size(); localNodeId++) {
                if (part[localNodeId] == proc) {
                    auto globalNodeId = mesh->data->globalNodeIds[localNodeId];
                    sendIds.push_back(globalNodeId);
                }
            }
            mp.Gatherv(sendIds, recvNodeIds, proc);
        }
        std::sort(recvNodeIds.begin(), recvNodeIds.end());
        return recvNodeIds;
    }

    inline void NodeBasedRedistributor::redistributeNodeMetaData(std::vector<long>& my_non_ghost_ids,
                                                                 std::vector<long>& my_ghost_ids) {
        std::map<long, int> global_to_local;
        for (unsigned int i = 0; i < mesh->data->globalNodeIds.size(); i++)
            global_to_local[mesh->data->globalNodeIds[i]] = i;
        for (int proc = 0; proc < mp.NumberOfProcesses(); ++proc) {
            std::vector<long> idsOfXyxsINeed;
            if (mp.Rank() == proc) {
                idsOfXyxsINeed = my_non_ghost_ids;
                idsOfXyxsINeed.insert(idsOfXyxsINeed.end(), my_ghost_ids.begin(), my_ghost_ids.end());
            }
            mp.Broadcast(idsOfXyxsINeed, proc);
            std::vector<double> sendXYZ;
            std::vector<long> sendGlobalNodeId;
            std::vector<int> sendAssociatedComponentId;
            for (auto globalNodeId : idsOfXyxsINeed) {
                if (amItheOwnerOfThisNode(globalNodeId, global_to_local)) {
                    int localNodeId = global_to_local[globalNodeId];
                    sendXYZ.push_back(mesh->data->xyz[3 * localNodeId + 0]);
                    sendXYZ.push_back(mesh->data->xyz[3 * localNodeId + 1]);
                    sendXYZ.push_back(mesh->data->xyz[3 * localNodeId + 2]);
                    sendGlobalNodeId.push_back(globalNodeId);
                    sendAssociatedComponentId.push_back(mesh->data->nodeComponentIds[localNodeId]);
                }
            }

            std::vector<double> gatheredXyz;
            mp.Gatherv(sendXYZ, gatheredXyz, proc);
            std::vector<long> gatheredIds;
            mp.Gatherv(sendGlobalNodeId, gatheredIds, proc);
            std::vector<int> gatheredComponentIds;
            mp.Gatherv(sendAssociatedComponentId, gatheredComponentIds, proc);
            if (mp.Rank() == proc) {
                recvXYZ.assign(gatheredXyz.size(), 0);
                recvAssociatedComponentIds.assign(gatheredComponentIds.size(), -1);
                for (unsigned int index = 0; index < gatheredIds.size(); index++) {
                    long globalNodeId = gatheredIds[index];
                    int localId = getLocalNodeId(globalNodeId, my_non_ghost_ids, my_ghost_ids);
                    for (int i = 0; i < 3; i++)
                        recvXYZ[3 * localId + i] = gatheredXyz[3 * index + i];
                    recvAssociatedComponentIds[localId] = gatheredComponentIds[index];
                }
            }
        }
    }

    inline bool NodeBasedRedistributor::amItheOwnerOfThisNode(long globalId, const std::map<long, int>& globalToLocal) {
        auto it = globalToLocal.find(globalId);
        if (globalToLocal.end() == it)
            return false;
        int localId = it->second;
        if (mesh->data->nodeOwnerRank[localId] == mp.Rank())
            return true;
        return false;
    }
    inline std::vector<int> NodeBasedRedistributor::redistributeTags(std::vector<long>& my_non_ghost_ids,
                                                                     std::vector<int>& cells,
                                                                     std::vector<int>& tags,
                                                                     int cellSize) {
        std::vector<int> recvCellTags;
        for (int proc = 0; proc < mp.NumberOfProcesses(); proc++) {
            std::vector<long> neededNodeIds;
            std::vector<int> sendCellTags;
            if (mp.Rank() == proc)
                neededNodeIds = my_non_ghost_ids;
            mp.Broadcast(neededNodeIds, proc);
            for (unsigned int i = 0; i < cells.size() / cellSize; i++) {
                if (iShouldSendThisSurfaceCell(&cells[cellSize * i], cellSize, neededNodeIds))
                    sendCellTags.push_back(tags[i]);
            }
            mp.Gatherv(sendCellTags, recvCellTags, proc);
        }
        return recvCellTags;
    }

    inline std::vector<long> NodeBasedRedistributor::redistributeCells(std::vector<long>& my_non_ghost_ids,
                                                                       std::vector<int>& cells, int cellSize) {
        std::vector<long> recvCells;

        for (int proc = 0; proc < mp.NumberOfProcesses(); proc++) {
            std::vector<long> neededNodeIds;
            std::vector<long> sendCellIds;
            if (mp.Rank() == proc)
                neededNodeIds = my_non_ghost_ids;
            mp.Broadcast(neededNodeIds, proc);
            for (unsigned int i = 0; i < cells.size() / cellSize; i++) {
                if (iShouldSendThisCell(&cells[cellSize * i], cellSize, neededNodeIds))
                    sendCellIds.push_back(i);
            }
            std::vector<long> sendCells;
            sendCells.reserve(cellSize * sendCellIds.size());
            for (auto id:sendCellIds)
                for (int j = 0; j < cellSize; j++)
                    sendCells.push_back(mesh->data->globalNodeIds[cells[cellSize * id + j]]);
            mp.Gatherv(sendCells, recvCells, proc);
        }
        return recvCells;
    }

    inline std::vector<long> NodeBasedRedistributor::redistributeSurfaceCells(std::vector<long>& my_non_ghost_ids,
                                                                              std::vector<int>& cells, int cellSize) {
        std::vector<long> sendCellIds;
        std::vector<long> recvCells;
        sendCellIds.reserve(cells.size());
        for (int proc = 0; proc < mp.NumberOfProcesses(); proc++) {
            sendCellIds.clear();
            std::vector<long> neededNodeIds;
            if (mp.Rank() == proc)
                neededNodeIds = my_non_ghost_ids;
            mp.Broadcast(neededNodeIds, proc);
            for (unsigned int i = 0; i < cells.size() / cellSize; i++) {
                if (iShouldSendThisSurfaceCell(&cells[cellSize * i], cellSize, neededNodeIds))
                    sendCellIds.push_back(i);
            }
            std::vector<long> sendCells;
            sendCells.reserve(sendCellIds.size());
            for (auto id:sendCellIds)
                for (int j = 0; j < cellSize; j++)
                    sendCells.push_back(mesh->data->globalNodeIds[cells[cellSize * id + j]]);
            mp.Gatherv(sendCells, recvCells, proc);
        }
        return recvCells;
    }

    inline bool NodeBasedRedistributor::iShouldSendThisCell(int* cell, int cellSize, const std::vector<long>& neededNodeIds) {
        if (mesh->data->nodeOwnerRank[cell[0]] != mp.Rank()) return false;
        for (int i = 0; i < cellSize; i++) {
            int localNodeId = cell[i];
            long globalNodeId = mesh->data->globalNodeIds[localNodeId];
            if (std::binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalNodeId))
                return true;
        }
        return false;
    }

    inline bool NodeBasedRedistributor::iShouldSendThisSurfaceCell(int* cell, int cellSize, const std::vector<long>& neededNodeIds) {
        if (mesh->data->nodeOwnerRank[cell[0]] != mp.Rank()) return false;
        for (int i = 0; i < cellSize; i++) {
            int localNodeId = cell[i];
            long globalNodeId = mesh->data->globalNodeIds[localNodeId];
            if (std::binary_search(neededNodeIds.begin(), neededNodeIds.end(), globalNodeId))
                return true;
        }
        return false;
    }

    inline std::vector<long> NodeBasedRedistributor::identifyGhostNodes(std::vector<long>& my_non_ghost_ids,
                                                                        std::vector<long>& recvTets,
                                                                        std::vector<long>& recvPyramids,
                                                                        std::vector<long>& recvPrisms,
                                                                        std::vector<long>& recvHexs) {
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
                                                                      const std::vector<long>& ids) const {
        std::vector<int> local_ids;
        for (auto id:ids)
            local_ids.push_back(global_to_local_map[id]);
        return local_ids;
    }

    inline int NodeBasedRedistributor::getLocalNodeId(long globalNodeId, std::vector<long>& my_non_ghost_ids,
                                                      std::vector<long>& my_ghost_ids) {
        int numberOfStuffs = my_non_ghost_ids.size();
        if (std::binary_search(my_non_ghost_ids.begin(), my_non_ghost_ids.end(), globalNodeId))
            return std::distance(my_non_ghost_ids.begin(),
                                 std::lower_bound(my_non_ghost_ids.begin(), my_non_ghost_ids.end(), globalNodeId));
        else if (std::binary_search(my_ghost_ids.begin(), my_ghost_ids.end(), globalNodeId))
            return std::distance(my_ghost_ids.begin(),
                                 std::lower_bound(my_ghost_ids.begin(), my_ghost_ids.end(), globalNodeId)) +
                   numberOfStuffs;
        else
            throw std::domain_error("Didn't find the id...");
    }
}
