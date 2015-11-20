#include <stdio.h>
#include <vector>
#include <memory>
#include <RangeLoop.h>
#include <ImportedUgrid.h>
#include <UgridReader.h>
#include <LinearPartitioner.h>
#include "ConfigurationReader.h"

inline std::shared_ptr<ParallelMesh> Parfait::ParallelMeshReader::readDistributedGrid(
        std::string configurationFileName) {
    ConfigurationReader configurationReader(configurationFileName);
    Configuration config = configurationReader.createConfiguration();
    std::vector<std::string> grid_filenames;
    std::vector<bool> is_grid_big_endian;
    for(int i=0;i<config.numberOfGrids();i++){
        grid_filenames.push_back(config.getFilename(i));
        is_grid_big_endian.push_back(config.isBigEndian(i));
    }
    ParallelMeshReader reader(grid_filenames, is_grid_big_endian);
    return reader.distributeGridsEvenly();
}

inline std::shared_ptr<ParallelMesh> Parfait::ParallelMeshReader::readDistributedGrid(
        std::vector<std::string> gridFiles,
        std::vector<bool> isBigEndian){
    ParallelMeshReader reader(gridFiles, isBigEndian);
    return reader.distributeGridsEvenly();
}

inline Parfait::ParallelMeshReader::ParallelMeshReader(std::vector<std::string> gridFiles_in,
                                                       std::vector<bool> isBigEndian_in)
        : isBigEndian(isBigEndian_in), 
          gridFiles(gridFiles_in),
          mesh(std::make_shared<ParallelMesh>())
{
    gridNodeMap.push_back(0);
    gridTriangleMap.push_back(0);
    gridQuadMap.push_back(0);
    gridTetMap.push_back(0);
    gridPyramidMap.push_back(0);
    gridPrismMap.push_back(0);
    gridHexMap.push_back(0);
    for (int i:range(gridFiles)) {
        int nnodes, ntri, nquad, ntet, npyr, nprism, nhex;
        UgridReader::readHeader(gridFiles[i], nnodes, ntri, nquad, ntet, npyr, nprism, nhex, isBigEndian[i]);
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
    if (MessagePasser::Rank() == 0) {
        printf("Composite rootDomain has:\n");
        printf("--nnodes        %lu\n", gridNodeMap.back());
        printf("--triangles     %lu\n", gridTriangleMap.back());
        printf("--quads         %lu\n", gridQuadMap.back());
        printf("--tets          %lu\n", gridTetMap.back());
        printf("--pyramids      %lu\n", gridPyramidMap.back());
        printf("--prisms        %lu\n", gridPrismMap.back());
        printf("--hexs          %lu\n", gridHexMap.back());
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
    for(long globalNodeId = procNodeMap[MessagePasser::Rank()]; globalNodeId < procNodeMap[MessagePasser::Rank()+1]; globalNodeId++){
        globalToLocalId[globalNodeId] = localNodeId++;
    }

    for(auto &id : mesh->connectivity->triangles){
        if(globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for(auto &id : mesh->connectivity->quads){
        if(globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }

    for(auto &id : mesh->connectivity->tets){
        if(globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for(auto &id : mesh->connectivity->pyramids){
        if(globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for(auto &id : mesh->connectivity->prisms){
        if(globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }
    for(auto &id : mesh->connectivity->hexes){
        if(globalToLocalId.count(id) == 0)
            globalToLocalId[id] = localNodeId++;
        id = globalToLocalId[id];
    }

    for(auto &pair : globalToLocalId){
        auto global = pair.first;
        auto local = pair.second;
        localToGlobalId[local] = global;
    }
}

inline void Parfait::ParallelMeshReader::distributeUgrid() {
    if (0 == MessagePasser::Rank())
        buildDistributionMaps();
    MessagePasser::Broadcast(procNodeMap, 0);

    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--nodes\n");
    distributeNodes();

    long globalNumberOfNodes = totalNumberOfNodes();
    auto myNodeRange = LinearPartitioner::getRangeForWorker(MessagePasser::Rank(),globalNumberOfNodes,MessagePasser::NumberOfProcesses());
    int nchunks = MessagePasser::NumberOfProcesses();

    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--triangles\n");
    distributeTriangles(myNodeRange,nchunks);
    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--quads\n");
    distributeQuads(myNodeRange,nchunks);
    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--tets\n");
    distributeTets(myNodeRange,nchunks);
    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--pyramids\n");
    distributePyramids(myNodeRange,nchunks);
    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--prisms\n");
    distributePrisms(myNodeRange,nchunks);
    if (MessagePasser::Rank() == 0)
        printf("Distributing ...\n--hexes\n");
    distributeHexs(myNodeRange,nchunks);

    mapNodesToLocalSpace();
    createLocalToGlobalNodeIdMap();

    createNodeOwnerships();
    createNodeComponentIds();



    if(MessagePasser::Rank() == 0)
        printf("Distributing ghost xyz\n");
    int nregular = mesh->countNodesAtDegree(0);
    std::vector<long> ghostIds(mesh->metaData->globalNodeIds.begin()+nregular,mesh->metaData->globalNodeIds.end());
    auto ghostXyz = getXyzForGhostNodes(ghostIds);
    mesh->metaData->xyz.insert(mesh->metaData->xyz.end(),ghostXyz.begin(),ghostXyz.end());
    if (MessagePasser::Rank() == 0)
        printf("Done Distributing ...\n");
}

inline void ParallelMeshReader::createNodeComponentIds() {
    mesh->metaData->nodeComponentIds = std::vector<int>(localToGlobalId.size());
    for(unsigned int localId = 0; localId < localToGlobalId.size(); localId++){
        auto globalId = mesh->metaData->globalNodeIds[localId];
        auto componentId = getOwningGridOfNode(globalId);
        mesh->metaData->nodeComponentIds[localId] = componentId;
    }
}
inline void ParallelMeshReader::createNodeOwnerships() {
    mesh->metaData->nodeOwnershipDegree = std::vector<int>(mesh->metaData->globalNodeIds.size());
    for(unsigned int localId = 0; localId < localToGlobalId.size(); localId++){
        if(localId < mesh->metaData->xyz.size() / 3)
            mesh->metaData->nodeOwnershipDegree[localId] = 0;
        else
            mesh->metaData->nodeOwnershipDegree[localId] = 1;
    }
}

inline void Parfait::ParallelMeshReader::buildDistributionMaps() {
    int totalNodes = 0;
    for (int i:range(gridFiles)) {
        int nnodes, ntri, nquad, ntet, npyr, nprism, nhex;
        UgridReader::readHeader(gridFiles[i], nnodes, ntri, nquad,
                                ntet, npyr, nprism, nhex, isBigEndian[i]);
        totalNodes += nnodes;
    }
    int nproc = MessagePasser::NumberOfProcesses();
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

inline std::shared_ptr<ParallelMesh> Parfait::ParallelMeshReader::distributeGridsEvenly() {
    distributeUgrid();
    return mesh;
}


inline void Parfait::ParallelMeshReader::distributeNodes() {
    if(MessagePasser::Rank() == 0) {
        int nproc = MessagePasser::NumberOfProcesses();
        mesh->metaData->xyz = getNodes(0,procNodeMap[1]);
        for(int proc=1;proc<nproc;proc++) {
            vector<double> nodeBuffer = getNodes(procNodeMap[proc],procNodeMap[proc+1]);
            MessagePasser::Send(nodeBuffer,proc);
        }
    }
    else
        MessagePasser::Recv(mesh->metaData->xyz,0);
}

inline std::set<int> ParallelMeshReader::getTargetProcessors(const std::vector<long> &transmitCell) {
    std::set<int> target_procs;
    for(const auto &id : transmitCell)
        target_procs.insert(getOwningProcOfNode(id));
    return target_procs;
}

inline std::vector<long> ParallelMeshReader::getCell(int cellLength, const std::vector<long> &cells, int cellId) const {
    std::vector<long> transmitCell;
    for(int i = 0; i < cellLength; i++){
        auto id = cells[cellLength*cellId + i];
        transmitCell.push_back(id);
    }
    return transmitCell;
}

inline bool isDoneSignal(const std::vector<long> &signal){
    return 1 == signal.size();
}

inline void Parfait::ParallelMeshReader::distributeTriangles(Parfait::LinearPartitioner::Range<long>& myNodeRange,
                                                        int nchunks) {
    long ntriangles = gridTriangleMap.back();
    for(int i =0; i <nchunks;++i){
        auto faces = getCellChunk(TRIANGLE, i, ntriangles,nchunks);
        auto tags = getTagChunk(TRIANGLE_TAG, i, ntriangles,nchunks);
        MessagePasser::Broadcast(faces,0);
        MessagePasser::Broadcast(tags,0);
        extractAndAppendFaces(3, faces,tags,mesh->connectivity->triangles,mesh->metaData->triangleTags,myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributeQuads(Parfait::LinearPartitioner::Range<long>& myNodeRange,
                                                             int nchunks) {
    long nquads = gridQuadMap.back();
    for(int i =0; i <nchunks;++i){
        auto faces = getCellChunk(QUAD, i, nquads,nchunks);
        auto tags = getTagChunk(QUAD_TAG, i, nquads,nchunks);
        MessagePasser::Broadcast(faces,0);
        MessagePasser::Broadcast(tags,0);
        extractAndAppendFaces(4, faces,tags,mesh->connectivity->quads,mesh->metaData->quadTags,myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributeTets(Parfait::LinearPartitioner::Range<long>& myNodeRange,
                                                        int nchunks) {
    long ntets = gridTetMap.back();
    for(int i =0; i <nchunks;++i){
        auto tetChunk = getCellChunk(TET, i,ntets,nchunks);
        MessagePasser::Broadcast(tetChunk,0);
        extractAndAppendCells(4,tetChunk,mesh->connectivity->tets,myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributePyramids(Parfait::LinearPartitioner::Range<long>& myNodeRange,
                                                        int nchunks) {
    long npyramids = gridPyramidMap.back();
    for(int i =0; i <nchunks;++i){
        auto chunk = getCellChunk(PYRAMID, i, npyramids,nchunks);
        MessagePasser::Broadcast(chunk,0);
        extractAndAppendCells(5, chunk,mesh->connectivity->pyramids,myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributePrisms(Parfait::LinearPartitioner::Range<long>& myNodeRange,
                                                        int nchunks) {
    long nprisms = gridPrismMap.back();
    for(int i =0; i <nchunks;++i){
        auto chunk = getCellChunk(PRISM, i, nprisms,nchunks);
        MessagePasser::Broadcast(chunk,0);
        extractAndAppendCells(6, chunk,mesh->connectivity->prisms,myNodeRange);
    }
}

inline void Parfait::ParallelMeshReader::distributeHexs(Parfait::LinearPartitioner::Range<long>& myNodeRange,
                                                          int nchunks) {
    long nhexs = gridHexMap.back();
    for(int i =0; i <nchunks;++i){
        auto chunk = getCellChunk(HEX, i, nhexs,nchunks);
        MessagePasser::Broadcast(chunk,0);
        extractAndAppendCells(8, chunk,mesh->connectivity->hexes,myNodeRange);
    }
}

inline std::vector<long> Parfait::ParallelMeshReader::getCellChunk(Parfait::ParallelMeshReader::CellType cellType,
                                                            int chunkId,long nCells,int nchunks){
    std::vector<long> chunk;
    if(MessagePasser::Rank() == 0) {
        auto range = LinearPartitioner::getRangeForWorker(chunkId, nCells, nchunks);
        switch(cellType){
            case TET: return getFromGrids(UgridReader::readTets, 4,gridTetMap, range.start, range.end,long());
            case PYRAMID: return getFromGrids(UgridReader::readPyramids,5,gridPyramidMap,range.start,range.end,long());
            case PRISM: return getFromGrids(UgridReader::readPrisms,6,gridPrismMap,range.start,range.end,long());
            case HEX: return getFromGrids(UgridReader::readHexs,8,gridHexMap,range.start,range.end,long());
            case TRIANGLE: return getFromGrids(UgridReader::readTriangles,3,gridTriangleMap,range.start,range.end,long());
            case QUAD: return getFromGrids(UgridReader::readQuads,4,gridQuadMap,range.start,range.end,long());
        }
        throw std::logic_error("Invalid cell type");
    }
    return chunk;
}

inline std::vector<int> Parfait::ParallelMeshReader::getTagChunk(Parfait::ParallelMeshReader::TagType tagType,
                                                                   int chunkId,long nCells,int nchunks){
    std::vector<int> chunk;
    if(MessagePasser::Rank() == 0) {
        auto range = LinearPartitioner::getRangeForWorker(chunkId, nCells, nchunks);
        switch(tagType){
            case TRIANGLE_TAG: return getFromGrids(UgridReader::readTriangleBoundaryTags,1,gridTriangleMap,range.start,range.end,int());
            case QUAD_TAG: return getFromGrids(UgridReader::readQuadBoundaryTags,1,gridQuadMap,range.start,range.end,int());
        }
        throw std::logic_error("Invalid tag type");
    }
    return chunk;
}

inline void Parfait::ParallelMeshReader::extractAndAppendCells(int cellSize,
                                                               const std::vector<long>& chunkCells,
                                                               std::vector<int>& saveCells,
                                                               const LinearPartitioner::Range<long>& myNodeRange){
    for(unsigned int i=0;i<chunkCells.size()/cellSize;++i){
        bool iOwnIt = false;
        for(int j=0;j<cellSize;++j)
            if(myNodeRange.owns(chunkCells[cellSize*i+j]))
                iOwnIt = true;
        if(iOwnIt){
            for(int j=0;j<cellSize;++j)
                saveCells.push_back(chunkCells[cellSize*i+j]);
        }
    }
}

inline void Parfait::ParallelMeshReader::extractAndAppendFaces(int cellSize,
                                                               const std::vector<long>& chunkFaces,
                                                               const std::vector<int>& chunkTags,
                                                               std::vector<int>& saveFaces,
                                                               std::vector<int>& saveTags,
                                                               const LinearPartitioner::Range<long>& myNodeRange){
    for(unsigned int i=0;i< chunkFaces.size()/cellSize;++i){
        bool iOwnIt = false;
        for(int j=0;j<cellSize;++j)
            if(myNodeRange.owns(chunkFaces[cellSize*i+j]))
                iOwnIt = true;
        if(iOwnIt){
            for(int j=0;j<cellSize;++j)
                saveFaces.push_back(chunkFaces[cellSize*i+j]);
            saveTags.push_back(chunkTags[i]);
        }
    }
}

inline int Parfait::ParallelMeshReader::getOwningProcOfNode(long id) {
    auto nnodes = gridNodeMap.back();
    return LinearPartitioner::getWorkerOfWorkItem(id, nnodes, MessagePasser::NumberOfProcesses());
}

inline int Parfait::ParallelMeshReader::getOwningGridOfNode(long globalId){
    return getOwningGridOfEntity(gridNodeMap, globalId);
}

inline int Parfait::ParallelMeshReader::getOwningGridOfEntity(std::vector<long> &gridCellMap, long globalId){
    for(unsigned int gridId = 0; gridId < gridCellMap.size() - 1; gridId++){
        auto start = gridCellMap[gridId];
        auto end = gridCellMap[gridId+1];
        if(globalId >= start and globalId < end)
            return gridId;
    }
    throw std::logic_error("Could not find component grid of cell");
}

inline long ParallelMeshReader::convertComponentNodeIdToGlobal(int id,int grid) const {
    return id + gridNodeMap[grid];
}

inline std::vector<double> Parfait::ParallelMeshReader::getNodes(long begin, long end) {
    using namespace UgridReader;
    std::vector<double> nodeBuffer(3*(end-begin),0.0);
    int firstGrid  = getFirstGrid(gridNodeMap,begin);
    int lastGrid   = getLastGrid(gridNodeMap,end);
    int beginIndex = getBeginIndex(gridNodeMap,begin);
    int endIndex   = getEndIndex(gridNodeMap,end);
    int positionInBuffer = 0;
    std::vector<double> tmp;
    if(firstGrid == lastGrid)
        tmp = readNodes(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
    else
        tmp = readNodes(gridFiles[firstGrid],beginIndex,
                        gridNodeMap[firstGrid+1]-gridNodeMap[firstGrid],isBigEndian[firstGrid]);
    for(double node : tmp)
        nodeBuffer[positionInBuffer++] = node;
    tmp.clear();

    // read all nodes from grids between first and last grid
    for(int i=firstGrid+1;i<lastGrid;i++)
    {
        //printf("--From grid %i: reading all nodes\n",i);
        tmp = readNodes(gridFiles[i],isBigEndian[i]);
        for(double node : tmp)
            nodeBuffer[positionInBuffer++] = node;
        tmp.clear();
    }
    // read nodes from last grid (start at zero and end at endIndex)
    if(lastGrid > firstGrid)
    {
        tmp.clear();
        tmp = readNodes(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
        for(double node : tmp)
            nodeBuffer[positionInBuffer++] = node;
    }

    return nodeBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getTriangleTags(long begin, long end) {
    using namespace UgridReader;
    std::vector<int> triangleTagBuffer(end-begin,0);
    int firstGrid  = getFirstGrid(gridTriangleMap,begin);
    int lastGrid   = getLastGrid(gridTriangleMap,end);
    int beginIndex = getBeginIndex(gridTriangleMap,begin);
    int endIndex   = getEndIndex(gridTriangleMap,end);
    int positionInBuffer = 0;
    vector<int> tmp;
    if(firstGrid == lastGrid)
    {
        // read triangles from the first grid (start at beginIndex and read to endIndex)
        tmp = readTriangleBoundaryTags(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
    }
    else
    {
        // read triangles from the first grid (start at beginIndex and read to the end of the file)
        tmp = readTriangleBoundaryTags(gridFiles[firstGrid],beginIndex,gridTriangleMap[firstGrid+1]
                                                                       -gridTriangleMap[firstGrid],isBigEndian[firstGrid]);
    }
    for(int tag : tmp)
        triangleTagBuffer[positionInBuffer++] = tag;
    tmp.clear();

    // read all triangles from grids between first and last grid
    for(int i=firstGrid+1;i<lastGrid;i++)
    {
        tmp = readTriangleBoundaryTags(gridFiles[i],isBigEndian[i]);
        for(int tag : tmp)
            triangleTagBuffer[positionInBuffer++] = tag;
        tmp.clear();
    }

    // read triangles from last grid (start at zero and end at endIndex)
    if(lastGrid > firstGrid)
    {
        tmp = readTriangleBoundaryTags(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
        for(int tag : tmp)
            triangleTagBuffer[positionInBuffer++] = tag;
    }
    return triangleTagBuffer;
}

inline std::vector<int> Parfait::ParallelMeshReader::getQuadTags(long begin, long end) {
    using namespace UgridReader;
    std::vector<int> quadTagBuffer(end-begin,0);
    int firstGrid  = getFirstGrid(gridQuadMap,begin);
    int lastGrid   = getLastGrid(gridQuadMap,end);
    int beginIndex = getBeginIndex(gridQuadMap,begin);
    int endIndex   = getEndIndex(gridQuadMap,end);
    int positionInBuffer = 0;
    vector<int> tmp;
    if(firstGrid == lastGrid)
    {
        // read quads from the first grid (start at beginIndex and read to endIndex)
        tmp = readQuadBoundaryTags(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
    }
    else
    {
        // read quads from the first grid (start at beginIndex and read to the end of the file)
        tmp = readQuadBoundaryTags(gridFiles[firstGrid],beginIndex,gridQuadMap[firstGrid+1]
                                                                   -gridQuadMap[firstGrid],isBigEndian[firstGrid]);
    }
    for(int tag : tmp)
        quadTagBuffer[positionInBuffer++] = tag;
    tmp.clear();

    // read all quads from grids between first and last grid
    for(int i=firstGrid+1;i<lastGrid;i++)
    {
        tmp = readQuadBoundaryTags(gridFiles[i],isBigEndian[i]);
        for(int tag : tmp)
            quadTagBuffer[positionInBuffer++] = tag;
        tmp.clear();
    }

    // read quads from last grid (start at zero and end at endIndex)
    if(lastGrid > firstGrid)
    {
        tmp = readQuadBoundaryTags(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
        for(int tag : tmp)
            quadTagBuffer[positionInBuffer++] = tag;
    }
    return quadTagBuffer;
}

template<typename ReturnType>
inline std::vector<ReturnType> Parfait::ParallelMeshReader::getFromGrids(
    std::vector<int>(*readingFunction)(std::string, int, int, bool),
    int objectSize,
    std::vector<long> &gridElementMap, long begin, long end,ReturnType constructor) {

    std::vector<ReturnType> buffer(objectSize*(end-begin),0);
    int firstGrid  = getFirstGrid(gridElementMap,begin);
    int lastGrid   = getLastGrid(gridElementMap,end);
    int beginIndex = getBeginIndex(gridElementMap,begin);
    int endIndex   = getEndIndex(gridElementMap,end);
    int positionInBuffer = 0;
    std::vector<int> tmp;
    if(firstGrid == lastGrid) {
        // read objects from the first grid (start at beginIndex and read to endIndex)
        tmp = readingFunction(gridFiles[firstGrid],beginIndex,endIndex,isBigEndian[firstGrid]);
    }
    else {
        // read objects from the first grid (start at beginIndex and read to the end of the file)
        tmp = readingFunction(gridFiles[firstGrid],beginIndex, gridElementMap[firstGrid+1]
                                                       - gridElementMap[firstGrid],isBigEndian[firstGrid]);
    }
    for(int x : tmp)
        buffer[positionInBuffer++] = x + gridNodeMap[firstGrid];
    tmp.clear();
    // read all objects from grids between first and last grid
    for(int i=firstGrid+1;i<lastGrid;i++) {
        tmp = readingFunction(gridFiles[i],0, gridElementMap[i+1]- gridElementMap[i],isBigEndian[i]);
        for(int x : tmp)
            buffer[positionInBuffer++] = x + gridNodeMap[i];
        tmp.clear();
    }

    // read objects from last grid (start at zero and end at endIndex)
    if(lastGrid > firstGrid) {
        tmp = readingFunction(gridFiles[lastGrid],0,endIndex,isBigEndian[lastGrid]);
        for(int x : tmp)
            buffer[positionInBuffer++] = x + gridNodeMap[lastGrid];
    }
    return buffer;
}

inline int Parfait::ParallelMeshReader::getFirstGrid(std::vector<long> &gridMap, long begin) {
    int ngrid = (int)gridFiles.size();
    for(int i=0;i<ngrid;i++)
        if(begin <= gridMap[i+1])
            return i;
    assert(false);
}

inline int Parfait::ParallelMeshReader::getLastGrid(std::vector<long> &gridMap, long end) {
    if(0==end){return 0;}
    int ngrid = (int)gridFiles.size();
    for(int i=ngrid-1;i>=0;i--)
        if(end >= gridMap[i])
            return i;
    assert(false);
}

inline int Parfait::ParallelMeshReader::getBeginIndex(std::vector<long> &gridMap, long begin) {
    int ngrid = (int)gridFiles.size();
    for(int i=0;i<ngrid;i++)
    {
        if(begin <= gridMap[i+1])
            return begin-gridMap[i];
    }
    assert(false);
}

inline int Parfait::ParallelMeshReader::getEndIndex(std::vector<long> &gridMap, long end) {
    if(0==end){return 0;}
    int ngrid = (int)gridFiles.size();
    for(int i=ngrid-1;i>=0;i--)
        if(end >= gridMap[i])
            return end-gridMap[i];
    assert(false);
}

inline long Parfait::ParallelMeshReader::totalNumberOfNodes() const{
    return gridNodeMap.back();
}

inline int Parfait::ParallelMeshReader::numberOfGrids() const{
    return gridFiles.size();
}

inline void ParallelMeshReader::createLocalToGlobalNodeIdMap() {
    for(unsigned int localId = 0; localId < localToGlobalId.size(); localId++){
        mesh->metaData->globalNodeIds.push_back(localToGlobalId[localId]);
    }
}

inline std::vector<double> ParallelMeshReader::getXyzForGhostNodes(std::vector<long>& ghostIds){
    std::vector<long> gatheredIds;
    std::vector<double> gatheredXyz;
    for(int proc=0;proc<MessagePasser::NumberOfProcesses();++proc){
        std::vector<long> requestedIds;
        if(MessagePasser::Rank() == proc)
            requestedIds = ghostIds;
        MessagePasser::Broadcast(requestedIds,proc);
        std::vector<long> responseIds;
        std::vector<double> responseXyz;
        for(long id:requestedIds){
            if(globalToLocalId.count(id) == 1){
                int localId = globalToLocalId[id];
                if(mesh->metaData->nodeOwnershipDegree[localId]==0) {
                    responseIds.push_back(id);
                    responseXyz.push_back(mesh->metaData->xyz[3 * localId + 0]);
                    responseXyz.push_back(mesh->metaData->xyz[3 * localId + 1]);
                    responseXyz.push_back(mesh->metaData->xyz[3 * localId + 2]);
                }
            }
        }
        MessagePasser::Gatherv(responseIds,gatheredIds,proc);
        MessagePasser::Gatherv(responseXyz,gatheredXyz,proc);
    }
    std::map<long,int> tmp_map;
    for(unsigned int i=0;i<ghostIds.size();++i)
        tmp_map.insert(std::make_pair(ghostIds[i],i));
    std::vector<double> ghostxyz(3*ghostIds.size(),99.0);
    for(unsigned int i=0;i<ghostIds.size();++i){
        int localId = tmp_map[ghostIds[i]];
        ghostxyz[3*localId+0] = gatheredXyz[3*i+0];
        ghostxyz[3*localId+1] = gatheredXyz[3*i+1];
        ghostxyz[3*localId+2] = gatheredXyz[3*i+2];
    }
    return ghostxyz;
}
