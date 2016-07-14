#pragma once

#include <vector>
#include <string>
#include <set>
#include <stdlib.h>
#include <MessagePasser.h>
#include "LinearPartitioner.h"
#include <ParallelImportedUgrid.h>
#include "ParallelMesh.h"

namespace Parfait {

  class ParallelMeshReader {
      template<class T> using vector = std::vector<T>;
      enum CellType{TET,PYRAMID,PRISM,HEX,TRIANGLE,QUAD};
      enum TagType{TRIANGLE_TAG,QUAD_TAG};

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

      std::shared_ptr<ParallelMeshBuilder> meshBuilder;

      void buildDistributionMaps();
      void distributeNodes();

      void distributeTriangles(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributeQuads(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributeTets(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributePyramids(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributePrisms(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributeHexs(LinearPartitioner::Range<long>& myNodeRange,int nchunks);
      void distributeUgrid();
      void mapNodesToLocalSpace();

      template<typename ReadingFunction,typename ReturnType>
      std::vector<ReturnType> getFromGrids(ReadingFunction readingFunction,
                                     int objectSize,
                                     std::vector<long> &gridElementMap,
                                     long begin, long end,bool isConnectivity,ReturnType constructor);
      int getFirstGrid(std::vector<long> &gridMap, long begin);
      int getLastGrid(std::vector<long> &gridMap, long end);
      int getBeginIndex(std::vector<long> &gridMap, long begin);
      int getEndIndex(std::vector<long> &gridMap, long end);

      int getOwningProcOfNode(long id);
      int getOwningGridOfNode(long id);
      int getOwningGridOfEntity(std::vector<long> &gridCellMap, long globalId);

      std::map<long, int> globalToLocalId;
      std::map<int, long> localToGlobalId;
      void createLocalToGlobalNodeIdMap();
      void createNodeOwnerships();
      void createNodeComponentIds();
      std::vector<double> getXyzForGhostNodes(std::vector<long>& ghostIds);

      void extractAndAppendCells(int cellSize,
                                 const std::vector<long>& chunkCells,
                                 std::vector<int>& saveCells,
                                 const LinearPartitioner::Range<long>& myNodeRange);
      void extractAndAppendFaces(int cellSize,
                                 const std::vector<long>& chunkFaces,
                                 const std::vector<int>& chunkTags,
                                 std::vector<int>& saveCells,
                                 std::vector<int>& saveTags,
                                 const LinearPartitioner::Range<long>& myNodeRange);
      std::vector<long> getCellChunk(CellType cellType,int chunkId,long nCells,int nChunks);
      std::vector<int> getTagChunk(TagType tagType,int chunkId,long nCells,int nChunks);

      void gatherGhostXyz();
  };
}

#include "ParallelMeshReader.hpp"