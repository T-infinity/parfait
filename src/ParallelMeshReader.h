#ifndef PARALLEL_MESH_READER_NAIVE_H
#define PARALLEL_MESH_READER_NAIVE_H

#include <vector>
#include <string>
#include <set>
#include <stdlib.h>
#include <MessagePasser.h>
#include <ImportedUgrid.h>

namespace Parfait {
  class ParallelMeshReader {
    template<class T> using vector = std::vector<T>;
  public:
    static ImportedUgrid readDistributedGrid(std::string configurationFileName);
    ParallelMeshReader(std::vector<std::string> gridFiles, std::vector<bool> isBigEndian);
    ImportedUgrid distributeGridsEvenly();
    std::vector<int> getGridNodeMap();
    std::vector<int> getProcNodeMap();
  private:
    std::vector<bool> isBigEndian;
    std::vector<std::string> gridFiles;
    std::vector<int> procNodeMap;

    std::vector<int> gridNodeMap;
    std::vector<int> gridCellMap;
    std::vector<int> gridTriangleMap;
    std::vector<int> gridQuadMap;
    std::vector<int> gridTetMap;
    std::vector<int> gridPyramidMap;
    std::vector<int> gridPrismMap;
    std::vector<int> gridHexMap;

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
    void distributeTriangles();
    void distributeUgrid();
    std::vector<double> getNodes(int begin, int end);
    std::vector<int> getTriangles(int begin, int end);
    std::vector<int> getTriangleTags(int begin, int end);
    std::vector<int> getQuads(int begin, int end);
    std::vector<int> getQuadTags(int begin, int end);
    std::vector<int> getTets(int begin, int end);
    std::vector<int> getPyramids(int begin, int end);
    std::vector<int> getPrisms(int begin, int end);
    std::vector<int> getHexs(int begin, int end);
    int getFirstGrid(std::vector<int> &gridMap, int begin);
    int getLastGrid(std::vector<int> &gridMap, int end);
    int getBeginIndex(std::vector<int> &gridMap, int begin);
    int getEndIndex(std::vector<int> &gridMap, int end);

    int convertComponentNodeIdToGlobal(int id,int grid) const;
    int getOwningProcOfNode(int id);

    void saveTriangle(const std::vector<int>& triangle);
  };

}

#include "ParallelMeshReader.hpp"

#endif
