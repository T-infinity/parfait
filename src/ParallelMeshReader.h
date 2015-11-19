#ifndef PARALLEL_MESH_READER_NAIVE_H
#define PARALLEL_MESH_READER_NAIVE_H

#include <vector>
#include <string>
#include <set>
#include <stdlib.h>
#include <MessagePasser.h>
#include "LinearPartitioner.h"
#include <ParallelImportedUgrid.h>
#include "ParallelMesh.h"

template <typename T>
class MessageBuilder;

namespace Parfait {
  class ParallelMeshReader {
      template<class T> using vector = std::vector<T>;
      enum CellType{TET,PYRAMID,PRISM,HEX};
  public:
      static std::shared_ptr<ParallelMesh> readDistributedGrid(std::string configurationFileName);
      static std::shared_ptr<ParallelMesh> readDistributedGrid(std::vector<std::string> gridFiles,
                                                                    std::vector<bool> isBigEndian);
      ParallelMeshReader(std::vector<std::string> gridFiles, std::vector<bool> isBigEndian);
      std::shared_ptr<ParallelMesh> distributeGridsEvenly();
      std::vector<long> getGridNodeMap();
      std::vector<long> getProcNodeMap();
      long totalNumberOfNodes() const;
      int numberOfGrids() const;

  public:
      std::vector<bool> isBigEndian;
      std::vector<std::string> gridFiles;
      std::vector<long> procNodeMap;

      std::vector<long> gridNodeMap;
      std::vector<long> gridTriangleMap;
      std::vector<long> gridQuadMap;
      std::vector<long> gridTetMap;
      std::vector<long> gridPyramidMap;
      std::vector<long> gridPrismMap;
      std::vector<long> gridHexMap;

      std::shared_ptr<ParallelMesh> mesh;

      void buildDistributionMaps();
      void distributeNodes();
      void debug_printing();

      template<typename CellGetter, typename CellSaver>
      void rootDistributeCells(int cellLength, std::vector<long> &gridCellMap,
                                                            CellGetter cellGetter, CellSaver cellSaver);
      template<typename CellGetter, typename TagGetter, typename CellSaver>
      void rootDistributeSurfaceCells(int cellLength, std::vector<long> &gridCellMap,
                                      CellGetter cellGetter, TagGetter tagGetter,
                                      CellSaver cellSaver);
      template<typename CellSaver>
      void nonRootRecvCells(int cellLength, CellSaver cellSaver);
      template<typename CellSaver>
      void nonRootRecvSurfaceCells(int cellLength, CellSaver cellSaver);
      template <class CellSaver>
      void sendTransmitCellToTargets(CellSaver cellSaver,
                                     MessageBuilder<long> &messageBuilder, const std::set<int> &target_procs,
                                     const vector<long> &transmitCell) const;

      void distributeTriangles();
      void distributeQuads();
      void distributeTets(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributePyramids(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributePrisms(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributeHexs(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributeUgrid();
      void mapNodesToLocalSpace();

      std::vector<double> getNodes(long begin, long end);
      std::vector<long> getTriangles(long begin, long end);
      std::vector<int> getTriangleTags(long begin, long end);
      std::vector<long> getQuads(long begin, long end);
      std::vector<int> getQuadTags(long begin, long end);
      std::vector<long> getTets(long begin, long end);
      std::vector<long> getPyramids(long begin, long end);
      std::vector<long> getPrisms(long begin, long end);
      std::vector<long> getHexs(long begin, long end);
      int getFirstGrid(std::vector<long> &gridMap, long begin);
      int getLastGrid(std::vector<long> &gridMap, long end);
      int getBeginIndex(std::vector<long> &gridMap, long begin);
      int getEndIndex(std::vector<long> &gridMap, long end);

      long convertComponentNodeIdToGlobal(int id, int grid) const;
      int getOwningProcOfNode(long id);
      int getOwningGridOfNode(long id);
      int getOwningGridOfEntity(std::vector<long> &gridCellMap, long globalId);


      void saveTriangle(std::vector<long> triangle);
      void saveQuad(std::vector<long> quad);
      void saveTet(const std::vector<long> &tet);
      void savePyramid(const std::vector<long> &pyramid);
      void savePrism(const std::vector<long> &prism);
      void saveHex(const std::vector<long> &hex);

      std::map<long, int> globalToLocalId;
      std::map<int, long> localToGlobalId;
      void createLocalToGlobalNodeIdMap();
      void createNodeOwnerships();
      void createNodeComponentIds();
      std::vector<double> getXyzForGhostNodes(std::vector<long>& ghostIds);

      std::set<int> getTargetProcessors(const vector<long> &transmitCell);
      std::vector<long> getCell(int cellLength, const vector<long> &cells, int cellId) const;
      void extractAndAppendCells(int cellSize,
                                 const std::vector<long>& chunkCells,
                                 std::vector<int>& saveCells,
                                 const LinearPartitioner::Range<long>& myNodeRange);
      std::vector<long> getCellChunk(CellType cellType,int chunkId,long nCells,int nChunks);
  };
}

#include "ParallelMeshReader.hpp"

#endif
