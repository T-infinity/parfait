#include <stdio.h>
#include <vector>
#include <memory>
#include "ImportedUgrid.h"
#include "UgridReader.h"
#include "LinearPartitioner.h"
#include "ConfigurationReader.h"

inline std::vector<int> findNodeOwnerRank(
        MessagePasser mp,
        std::vector<long>& have, std::set<long>& need,
        const std::map<long, int>& global_to_local) {

    std::vector<int> node_owner(global_to_local.size());
    for (auto global_node_id : have) {
        auto local = global_to_local.at(global_node_id);
        node_owner[local] = mp.Rank();
    }

    for (int r = 0; r < mp.NumberOfProcesses(); r++) {
        std::vector<long> transmit;
        if (r == mp.Rank()) {
            transmit = have;
        }
        mp.Broadcast(transmit, r);
        for (auto global_node_id : transmit) {
            if (need.count(global_node_id)) {
                int local = global_to_local.at(global_node_id);
                node_owner[local] = r;
            }
        }
    }

    return node_owner;
}

inline std::vector<int> setNodeOwnerRank(
        MessagePasser mp,
        const std::vector<int>& ownership_degree,
        const std::vector<long>& globalNodeIds) {

    std::vector<long> have;
    std::set<long> need;
    std::map<long, int> global_to_local;
    for (int i = 0; i < globalNodeIds.size(); i++) {
        if (ownership_degree[i] == 0)
            have.push_back(globalNodeIds[i]);
        else
            need.insert(globalNodeIds[i]);
        global_to_local[globalNodeIds[i]] = i;
    }

    return findNodeOwnerRank(mp, have, need, global_to_local);
}

inline void Parfait::ParallelMeshReader::createNodeOwnerRank() {
    std::vector<long> have;
    std::set<long> need;
    std::map<long, int> global_to_local;
    int num_owned = meshBuilder->data->xyz.size() / 3;
    for (int local = 0; local < num_owned; local++) {
        auto global = localToGlobalId[local];
        have.push_back(global);
        global_to_local[global] = local;
    }
    for (int local = num_owned; local < localToGlobalId.size(); local++) {
        auto global = localToGlobalId[local];
        need.insert(global);
        global_to_local[global] = local;
    }
    meshBuilder->data->nodeOwnerRank = findNodeOwnerRank(mp, have, need, global_to_local);
}

inline std::shared_ptr<Parfait::ParallelMesh> Parfait::ParallelMeshReader::readDistributedGrid(
        MessagePasser mp,
        std::string configurationFileName) {
    ConfigurationReader configurationReader(configurationFileName);
    Configuration config = configurationReader.createConfiguration();
    std::vector<std::string> grid_filenames;
    std::vector<bool> is_grid_big_endian;
    for (int i = 0; i < config.numberOfGrids(); i++) {
        grid_filenames.push_back(config.getFilename(i));
        is_grid_big_endian.push_back(config.isBigEndian(i));
    }
    ParallelMeshReader reader(mp, grid_filenames, is_grid_big_endian);
    return reader.distributeGridsEvenly();
}

inline std::shared_ptr<Parfait::ParallelMesh> Parfait::ParallelMeshReader::readDistributedGrid(
        MessagePasser mp,
        std::vector<std::string> gridFiles,
        std::vector<bool> isBigEndian) {
    ParallelMeshReader reader(mp, gridFiles, isBigEndian);
    return reader.distributeGridsEvenly();
}

inline Parfait::ParallelMeshReader::ParallelMeshReader(
        MessagePasser mp, std::vector<std::string> gridFiles_in,
        std::vector<bool> isBigEndian_in)
        : mp(mp),
          isBigEndian(isBigEndian_in),
          gridFiles(gridFiles_in),
          meshBuilder(std::make_shared<ParallelMeshBuilder>(mp)) {
    gridNodeMap.push_back(0);
    gridTriangleMap.push_back(0);
    gridQuadMap.push_back(0);
    gridTetMap.push_back(0);
    gridPyramidMap.push_back(0);
    gridPrismMap.push_back(0);
    gridHexMap.push_back(0);
    for (int i = 0; i < gridFiles.size(); i++) {
        int nnodes, ntri, nquad, ntet, npyr, nprism, nhex;
        if (mp.Rank() == 0)
            UgridReader::readHeader(gridFiles[i], nnodes, ntri, nquad, ntet, npyr, nprism, nhex, isBigEndian[i]);
        mp.Broadcast(nnodes, 0);
        mp.Broadcast(ntri, 0);
        mp.Broadcast(nquad, 0);
        mp.Broadcast(ntet, 0);
        mp.Broadcast(npyr, 0);
        mp.Broadcast(nprism, 0);
        mp.Broadcast(nhex, 0);
        gridNodeMap.push_back(nnodes);
        gridTriangleMap.push_back(ntri);
        gridQuadMap.push_back(nquad);
        gridTetMap.push_back(ntet);
        gridPyramidMap.push_back(npyr);
        gridPrismMap.push_back(nprism);
        gridHexMap.push_back(nhex);
    }

    for (int i = 2; i < (int) gridNodeMap.size(); i++) {
        gridNodeMap[i] += gridNodeMap[i - 1];
        gridTriangleMap[i] += gridTriangleMap[i - 1];
        gridQuadMap[i] += gridQuadMap[i - 1];
        gridTetMap[i] += gridTetMap[i - 1];
        gridPyramidMap[i] += gridPyramidMap[i - 1];
        gridPrismMap[i] += gridPrismMap[i - 1];
        gridHexMap[i] += gridHexMap[i - 1];
    }
}

inline std::vector<long> Parfait::ParallelMeshReader::getGridNodeMap() {
    return gridNodeMap;
}

inline std::vector<long> Parfait::ParallelMeshReader::getProcNodeMap() {
    return procNodeMap;
}

inline void Parfait::ParallelMeshReader::mapNodesToLocalSpace() {
    int localNodeId = 0;
    for (long globalNodeId = procNodeMap[mp.Rank()]; globalNodeId < procNodeMap[mp.Rank() + 1]; globalNodeId++) {
        globalToLocalId[globalNodeId] = localNodeId++;
    }

    for (auto& id : meshBuilder->data->triangles) {
        if (globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for (auto& id : meshBuilder->data->quads) {
        if (globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }

    for (auto& id : meshBuilder->data->tets) {
        if (globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for (auto& id : meshBuilder->data->pyramids) {
        if (globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for (auto& id : meshBuilder->data->prisms) {
        if (globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for (auto& id : meshBuilder->data->hexs) {
        if (globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }

    for (auto& pair : globalToLocalId) {
        auto global = pair.first;
        auto local = pair.second;
        localToGlobalId[local] = global;
    }
}

inline void Parfait::ParallelMeshReader::distributeUgrid() {
    if (0 == mp.Rank())
        buildDistributionMaps();
    mp.Broadcast(procNodeMap, 0);

    distributeNodes();

    long globalNumberOfNodes = totalNumberOfNodes();
    auto myNodeRange = LinearPartitioner::getRangeForWorker(mp.Rank(),
                                                            globalNumberOfNodes,
                                                            mp.NumberOfProcesses());
    int nchunks = mp.NumberOfProcesses();

    distributeTriangles(myNodeRange, nchunks);
    distributeQuads(myNodeRange, nchunks);
    distributeTets(myNodeRange, nchunks);
    distributePyramids(myNodeRange, nchunks);
    distributePrisms(myNodeRange, nchunks);
    distributeHexs(myNodeRange, nchunks);

    mapNodesToLocalSpace();
    createLocalToGlobalNodeIdMap();

    createNodeComponentIds();
    createNodeOwnerRank();
    gatherGhostXyz();
}

inline void Parfait::ParallelMeshReader::gatherGhostXyz() {
    int nregular = 0;
    int number_of_nodes = int(meshBuilder->data->xyz.size() / 3);
    for (int i = 0; i < number_of_nodes; ++i)
        if (meshBuilder->data->nodeOwnerRank[i] == mp.Rank())
            nregular++;
    std::vector<long> ghostIds(meshBuilder->data->globalNodeIds.begin() + nregular,
                               meshBuilder->data->globalNodeIds.end());
    auto ghostXyz = getXyzForGhostNodes(ghostIds);
    meshBuilder->data->xyz.insert(meshBuilder->data->xyz.end(), ghostXyz.begin(), ghostXyz.end());
}

inline void Parfait::ParallelMeshReader::createNodeComponentIds() {
    meshBuilder->data->nodeComponentIds = std::vector<int>(localToGlobalId.size());
    for (unsigned int localId = 0; localId < localToGlobalId.size(); localId++) {
        auto globalId = meshBuilder->data->globalNodeIds[localId];
        auto componentId = getOwningGridOfNode(globalId);
        meshBuilder->data->nodeComponentIds[localId] = componentId;
    }
}

inline void Parfait::ParallelMeshReader::buildDistributionMaps() {
    int totalNodes = 0;
    for (int i = 0; i < gridFiles.size(); i++) {
        int nnodes, ntri, nquad, ntet, npyr, nprism, nhex;
        UgridReader::readHeader(gridFiles[i], nnodes, ntri, nquad,
                                ntet, npyr, nprism, nhex, isBigEndian[i]);
        totalNodes += nnodes;
    }
    int nproc = mp.NumberOfProcesses();
    procNodeMap.assign(nproc, 0);

    for (int i = 0; i < nproc; i++) {
        procNodeMap[i] = totalNodes / nproc;
    }
    for (int i = 0; i < totalNodes % nproc; i++)
        procNodeMap[i]++;
    procNodeMap.insert(procNodeMap.begin(), 0);
    for (int i = 2; i < nproc + 1; i++) {
        procNodeMap[i] += procNodeMap[i - 1];
    }
}

inline std::shared_ptr<Parfait::ParallelMesh> Parfait::ParallelMeshReader::distributeGridsEvenly() {
    distributeUgrid();
    return meshBuilder->exportMesh();
}

inline void Parfait::ParallelMeshReader::distributeNodes() {
    if (mp.Rank() == 0) {
        int nproc = mp.NumberOfProcesses();
        std::vector<double> (* f)(std::string, int, int, bool);
        f = UgridReader::readNodes;
        meshBuilder->data->xyz = getFromGrids(f, 3, gridNodeMap, 0, procNodeMap[1], false, double());
        for (int proc = 1; proc < nproc; proc++) {
            vector<double> nodeBuffer = getFromGrids(f, 3, gridNodeMap, procNodeMap[proc], procNodeMap[proc + 1], false,
                                                     double());
            mp.Send(nodeBuffer, proc);
        }
    } else
        mp.Recv(meshBuilder->data->xyz, 0);
}

inline void Parfait::ParallelMeshReader::distributeTriangles(
        Parfait::LinearPartitioner::Range<long>& myNodeRange,
        int nchunks) {
    long ntriangles = gridTriangleMap.back();
    for (int i = 0; i < nchunks; ++i) {
        auto faces = getCellChunk(TRIANGLE, i, ntriangles, nchunks);
        auto tags = getTagChunk(TRIANGLE_TAG, i, ntriangles, nchunks);
        mp.Broadcast(faces, 0);
        mp.Broadcast(tags, 0);
        extractAndAppendFaces(3, faces, tags, meshBuilder->data->triangles, meshBuilder->data->triangleTags,
                              myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributeQuads(
        Parfait::LinearPartitioner::Range<long>& myNodeRange,
        int nchunks) {
    long nquads = gridQuadMap.back();
    for (int i = 0; i < nchunks; ++i) {
        auto faces = getCellChunk(QUAD, i, nquads, nchunks);
        auto tags = getTagChunk(QUAD_TAG, i, nquads, nchunks);
        mp.Broadcast(faces, 0);
        mp.Broadcast(tags, 0);
        extractAndAppendFaces(4, faces, tags, meshBuilder->data->quads, meshBuilder->data->quadTags, myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributeTets(
        Parfait::LinearPartitioner::Range<long>& myNodeRange,
        int nchunks) {
    long ntets = gridTetMap.back();
    for (int i = 0; i < nchunks; ++i) {
        auto tetChunk = getCellChunk(TET, i, ntets, nchunks);
        mp.Broadcast(tetChunk, 0);
        extractAndAppendCells(4, tetChunk, meshBuilder->data->tets, myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributePyramids(
        Parfait::LinearPartitioner::Range<long>& myNodeRange,
        int nchunks) {
    long npyramids = gridPyramidMap.back();
    for (int i = 0; i < nchunks; ++i) {
        auto chunk = getCellChunk(PYRAMID, i, npyramids, nchunks);
        mp.Broadcast(chunk, 0);
        extractAndAppendCells(5, chunk, meshBuilder->data->pyramids, myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributePrisms(
        Parfait::LinearPartitioner::Range<long>& myNodeRange,
        int nchunks) {
    long nprisms = gridPrismMap.back();
    for (int i = 0; i < nchunks; ++i) {
        auto chunk = getCellChunk(PRISM, i, nprisms, nchunks);
        mp.Broadcast(chunk, 0);
        extractAndAppendCells(6, chunk, meshBuilder->data->prisms, myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributeHexs(
        Parfait::LinearPartitioner::Range<long>& myNodeRange,
        int nchunks) {
    long nhexs = gridHexMap.back();
    for (int i = 0; i < nchunks; ++i) {
        auto chunk = getCellChunk(HEX, i, nhexs, nchunks);
        mp.Broadcast(chunk, 0);
        extractAndAppendCells(8, chunk, meshBuilder->data->hexs, myNodeRange);
    }
}

inline std::vector<long> Parfait::ParallelMeshReader::getCellChunk(
        Parfait::ParallelMeshReader::CellType cellType,
        int chunkId, long nCells, int nchunks) {
    std::vector<long> chunk;
    if (mp.Rank() == 0) {
        auto range = LinearPartitioner::getRangeForWorker(chunkId, nCells, nchunks);
        std::vector<int> (* f)(std::string, int, int, bool);
        switch (cellType) {
            case TET:
                f = UgridReader::readTets;
                return getFromGrids(f, 4, gridTetMap, range.start, range.end, true, long());
            case PYRAMID:
                f = UgridReader::readPyramids;
                return getFromGrids(f, 5, gridPyramidMap, range.start, range.end, true, long());
            case PRISM:
                f = UgridReader::readPrisms;
                return getFromGrids(f, 6, gridPrismMap, range.start, range.end, true, long());
            case HEX:
                f = UgridReader::readHexs;
                return getFromGrids(f, 8, gridHexMap, range.start, range.end, true, long());
            case TRIANGLE:
                f = UgridReader::readTriangles;
                return getFromGrids(f, 3, gridTriangleMap, range.start, range.end, true, long());
            case QUAD:
                f = UgridReader::readQuads;
                return getFromGrids(f, 4, gridQuadMap, range.start, range.end, true, long());
        }
        throw std::logic_error("Invalid cell type");
    }
    return chunk;
}

inline std::vector<int> Parfait::ParallelMeshReader::getTagChunk(
        Parfait::ParallelMeshReader::TagType tagType,
        int chunkId, long nCells, int nchunks) {
    std::vector<int> chunk;
    if (mp.Rank() == 0) {
        auto range = LinearPartitioner::getRangeForWorker(chunkId, nCells, nchunks);
        std::vector<int> (* f)(std::string, int, int, bool);
        switch (tagType) {
            case TRIANGLE_TAG:
                f = UgridReader::readTriangleBoundaryTags;
                return getFromGrids(f, 1, gridTriangleMap, range.start, range.end, false, int());
            case QUAD_TAG:
                f = UgridReader::readQuadBoundaryTags;
                return getFromGrids(f, 1, gridQuadMap, range.start, range.end, false, int());
        }
        throw std::logic_error("Invalid tag type");
    }
    return chunk;
}

inline void Parfait::ParallelMeshReader::extractAndAppendCells(
        int cellSize,
        const std::vector<long>& chunkCells,
        std::vector<int>& saveCells,
        const LinearPartitioner::Range<long>& myNodeRange) {
    for (unsigned int i = 0; i < chunkCells.size() / cellSize; ++i) {
        bool iOwnIt = false;
        for (int j = 0; j < cellSize; ++j)
            if (myNodeRange.owns(chunkCells[cellSize * i + j]))
                iOwnIt = true;
        if (iOwnIt) {
            for (int j = 0; j < cellSize; ++j)
                saveCells.push_back(chunkCells[cellSize * i + j]);
        }
    }
}

inline void Parfait::ParallelMeshReader::extractAndAppendFaces(
        int cellSize,
        const std::vector<long>& chunkFaces,
        const std::vector<int>& chunkTags,
        std::vector<int>& saveFaces,
        std::vector<int>& saveTags,
        const LinearPartitioner::Range<long>& myNodeRange) {
    for (unsigned int i = 0; i < chunkFaces.size() / cellSize; ++i) {
        bool iOwnIt = false;
        for (int j = 0; j < cellSize; ++j)
            if (myNodeRange.owns(chunkFaces[cellSize * i + j]))
                iOwnIt = true;
        if (iOwnIt) {
            for (int j = 0; j < cellSize; ++j)
                saveFaces.push_back(chunkFaces[cellSize * i + j]);
            saveTags.push_back(chunkTags[i]);
        }
    }
}

inline int Parfait::ParallelMeshReader::getOwningProcOfNode(long id) {
    auto nnodes = gridNodeMap.back();
    return LinearPartitioner::getWorkerOfWorkItem(id, nnodes, mp.NumberOfProcesses());
}

inline int Parfait::ParallelMeshReader::getOwningGridOfNode(long globalId) {
    return getOwningGridOfEntity(gridNodeMap, globalId);
}

inline int Parfait::ParallelMeshReader::getOwningGridOfEntity(std::vector<long>& gridCellMap, long globalId) {
    for (unsigned int gridId = 0; gridId < gridCellMap.size() - 1; gridId++) {
        auto start = gridCellMap[gridId];
        auto end = gridCellMap[gridId + 1];
        if (globalId >= start and globalId < end)
            return gridId;
    }
    throw std::logic_error("Could not find component grid of cell");
}

template<typename ReadingFunction, typename ReturnType>
inline std::vector<ReturnType> Parfait::ParallelMeshReader::getFromGrids(
        ReadingFunction readingFunction,
        int objectSize,
        std::vector<long>& gridElementMap, long begin, long end, bool isConnectivity, ReturnType constructor) {

    std::vector<ReturnType> buffer(objectSize * (end - begin), 0);
    int firstGrid = getFirstGrid(gridElementMap, begin);
    int lastGrid = getLastGrid(gridElementMap, end);
    int beginIndex = getBeginIndex(gridElementMap, begin);
    int endIndex = getEndIndex(gridElementMap, end);
    int positionInBuffer = 0;
    if (firstGrid == lastGrid) {
        // read objects from the first grid (start at beginIndex and read to endIndex)
        auto tmp = readingFunction(gridFiles[firstGrid], beginIndex, endIndex, isBigEndian[firstGrid]);
        if (isConnectivity)
            for (auto& x : tmp)
                x += gridNodeMap[firstGrid];
        for (auto& x : tmp)
            buffer[positionInBuffer++] = x;
    } else {
        // read objects from the first grid (start at beginIndex and read to the end of the file)
        auto tmp = readingFunction(gridFiles[firstGrid], beginIndex, gridElementMap[firstGrid + 1]
                                                                     - gridElementMap[firstGrid],
                                   isBigEndian[firstGrid]);
        if (isConnectivity)
            for (auto& x : tmp)
                x += gridNodeMap[firstGrid];
        for (auto& x : tmp)
            buffer[positionInBuffer++] = x;
    }
    // read all objects from grids between first and last grid
    for (int i = firstGrid + 1; i < lastGrid; i++) {
        auto tmp = readingFunction(gridFiles[i], 0, gridElementMap[i + 1] - gridElementMap[i], isBigEndian[i]);
        if (isConnectivity)
            for (auto& x : tmp)
                x += gridNodeMap[i];
        for (auto& x : tmp)
            buffer[positionInBuffer++] = x;
    }

    // read objects from last grid (start at zero and end at endIndex)
    if (lastGrid > firstGrid) {
        auto tmp = readingFunction(gridFiles[lastGrid], 0, endIndex, isBigEndian[lastGrid]);
        if (isConnectivity)
            for (auto& x : tmp)
                x += gridNodeMap[lastGrid];
        for (auto& x : tmp)
            buffer[positionInBuffer++] = x;
    }
    return buffer;
}

inline int Parfait::ParallelMeshReader::getFirstGrid(std::vector<long>& gridMap, long begin) {
    int ngrid = (int) gridFiles.size();
    for (int i = 0; i < ngrid; i++)
        if (begin <= gridMap[i + 1])
            return i;
    assert(false);
}

inline int Parfait::ParallelMeshReader::getLastGrid(std::vector<long>& gridMap, long end) {
    if (0 == end) { return 0; }
    int ngrid = (int) gridFiles.size();
    for (int i = ngrid - 1; i >= 0; i--)
        if (end >= gridMap[i])
            return i;
    assert(false);
}

inline int Parfait::ParallelMeshReader::getBeginIndex(std::vector<long>& gridMap, long begin) {
    int ngrid = (int) gridFiles.size();
    for (int i = 0; i < ngrid; i++) {
        if (begin <= gridMap[i + 1])
            return begin - gridMap[i];
    }
    assert(false);
}

inline int Parfait::ParallelMeshReader::getEndIndex(std::vector<long>& gridMap, long end) {
    if (0 == end) { return 0; }
    int ngrid = (int) gridFiles.size();
    for (int i = ngrid - 1; i >= 0; i--)
        if (end >= gridMap[i])
            return end - gridMap[i];
    assert(false);
}

inline long Parfait::ParallelMeshReader::totalNumberOfNodes() const {
    return gridNodeMap.back();
}

inline int Parfait::ParallelMeshReader::numberOfGrids() const {
    return gridFiles.size();
}

inline void Parfait::ParallelMeshReader::createLocalToGlobalNodeIdMap() {
    for (unsigned int localId = 0; localId < localToGlobalId.size(); localId++) {
        meshBuilder->data->globalNodeIds.push_back(localToGlobalId[localId]);
    }
}

inline std::vector<double> Parfait::ParallelMeshReader::getXyzForGhostNodes(std::vector<long>& ghostIds) {
    std::vector<long> gatheredIds;
    std::vector<double> gatheredXyz;
    for (int proc = 0; proc < mp.NumberOfProcesses(); ++proc) {
        std::vector<long> requestedIds;
        if (mp.Rank() == proc)
            requestedIds = ghostIds;
        mp.Broadcast(requestedIds, proc);
        std::vector<long> responseIds;
        std::vector<double> responseXyz;
        for (long id:requestedIds) {
            if (globalToLocalId.count(id) == 1) {
                int localId = globalToLocalId[id];
                if (meshBuilder->data->nodeOwnerRank[localId] == mp.Rank()) {
                    responseIds.push_back(id);
                    responseXyz.push_back(meshBuilder->data->xyz[3 * localId + 0]);
                    responseXyz.push_back(meshBuilder->data->xyz[3 * localId + 1]);
                    responseXyz.push_back(meshBuilder->data->xyz[3 * localId + 2]);
                }
            }
        }
        mp.Gatherv(responseIds, gatheredIds, proc);
        mp.Gatherv(responseXyz, gatheredXyz, proc);
    }
    std::map<long, int> tmp_map;
    for (unsigned int i = 0; i < ghostIds.size(); ++i)
        tmp_map.insert(std::make_pair(ghostIds[i], i));
    std::vector<double> ghostxyz(3 * ghostIds.size(), 99.0);
    for (unsigned int i = 0; i < ghostIds.size(); ++i) {
        int localId = tmp_map[ghostIds[i]];
        ghostxyz[3 * localId + 0] = gatheredXyz[3 * i + 0];
        ghostxyz[3 * localId + 1] = gatheredXyz[3 * i + 1];
        ghostxyz[3 * localId + 2] = gatheredXyz[3 * i + 2];
    }
    return ghostxyz;
}
