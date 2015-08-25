#ifndef PARALLEL_MESH_READER_NAIVE_H
#define PARALLEL_MESH_READER_NAIVE_H

#include <vector>
#include <string>
#include <set>
#include <stdlib.h>
#include <MessagePasser.h>
#include <ParallelImportedUgrid.h>

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

      std::vector<double> myNodes;
      std::vector<int> myTriangles;
      std::vector<int> myQuads;
      std::vector<int> myTets;
      std::vector<int> myPyramids;
      std::vector<int> myPrisms;
      std::vector<int> myHexs;
      std::vector<int> myTriangleTags;
      std::vector<int> myQuadTags;

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

      void distributeTriangles();
      void distributeQuads();
      void distributeTets();
      void distributePyramids();
      void distributePrisms();
      void distributeHexs();
      void distributeUgrid();
      void mapNodesToLocalSpace();
      std::vector<double> getNodes(int begin, int end);
      std::vector<long> getTriangles(int begin, int end);
      std::vector<int> getTriangleTags(int begin, int end);
      std::vector<long> getQuads(int begin, int end);
      std::vector<int> getQuadTags(int begin, int end);
      std::vector<long> getTets(int begin, int end);
      std::vector<long> getPyramids(int begin, int end);
      std::vector<long> getPrisms(int begin, int end);
      std::vector<long> getHexs(int begin, int end);
      int getFirstGrid(std::vector<long> &gridMap, int begin);
      int getLastGrid(std::vector<long> &gridMap, int end);
      int getBeginIndex(std::vector<long> &gridMap, int begin);
      int getEndIndex(std::vector<long> &gridMap, int end);

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
  };
}

#include "ParallelMeshReader.hpp"

#endif
