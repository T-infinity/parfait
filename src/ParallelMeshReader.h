#ifndef PARALLEL_MESH_READER_NAIVE_H
#define PARALLEL_MESH_READER_NAIVE_H

#include <vector>
#include <string>
#include <set>
#include <stdlib.h>
#include <MessagePasser.h>
#include <ParallelImportedUgrid.h>
#include "MeshBasicParallel.h"

template <typename T>
class MessageBuilder;

namespace Parfait {
  class ParallelMeshReader {
      template<class T> using vector = std::vector<T>;
  public:
      static ParallelImportedUgrid readDistributedGrid(std::string configurationFileName);
      static ParallelImportedUgrid readDistributedGrid(std::vector<std::string> gridFiles, std::vector<bool> isBigEndian);
      ParallelMeshReader(std::vector<std::string> gridFiles, std::vector<bool> isBigEndian);
      ParallelImportedUgrid distributeGridsEvenly();
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

      MeshBasicParallel mesh;

      std::vector<double> myNodes;
      std::vector<int> myTriangles;
      std::vector<int> myQuads;
      std::vector<int> myTets;
      std::vector<int> myPyramids;
      std::vector<int> myPrisms;
      std::vector<int> myHexs;
      std::vector<int> myTriangleTags;
      std::vector<int> myQuadTags;

      std::vector<long> myGlobalNodeIds;
      std::vector<int> myOwnershipDegree;
      std::vector<int> myNodeComponentIds;

      void buildDistributionMaps();
      void distributeNodes();

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
      void distributeTets();
      void distributePyramids();
      void distributePrisms();
      void distributeHexs();
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

      std::set<int> getTargetProcessors(const vector<long> &transmitCell);
      std::vector<long> getCell(int cellLength, const vector<long> &cells, int cellId) const;
  };


}

#include "ParallelMeshReader.hpp"

#endif
