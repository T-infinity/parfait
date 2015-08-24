#ifndef PARFAIT_PARALLELIMPORTEDUGRID_HPP
#define PARFAIT_PARALLELIMPORTEDUGRID_HPP
namespace Parfait{
  inline ParallelImportedUgrid::ParallelImportedUgrid() :
          ImportedUgrid({},{},{},{},{},{},{},{},{},{},{}){
  }

  inline ParallelImportedUgrid::ParallelImportedUgrid(long globalNodeCount_in,
                                                      std::vector<long> globalNodeIds_in,
                                                      std::vector<double> nodes, std::vector<int> triangles,
                                                      std::vector<int> quads, std::vector<int> tets, std::vector<int> pyramids,
                                                      std::vector<int> prisms, std::vector<int> hexs,
                                                      std::vector<int> triangleTags, std::vector<int> quadTags,
                                                      std::vector<int> triangleBoundaryConditions,
                                                      std::vector<int> quadBoundaryConditions) :
          ImportedUgrid(nodes, triangles, quads, tets, pyramids, prisms, hexs, triangleTags, quadTags,
                        triangleBoundaryConditions,
                        quadBoundaryConditions),
          globalNodeCount(globalNodeCount_in),
          globalNodeIds(globalNodeIds_in){
  }
  inline long ParallelImportedUgrid::globalNumberOfNodes() const{
      return globalNodeCount;
  }
  inline long ParallelImportedUgrid::getGlobalNodeId(long localId) const{
      return globalNodeIds[localId];
  }


}
#endif 
