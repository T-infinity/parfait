#ifndef PARFAIT_PARALLELIMPORTEDUGRID_HPP
#define PARFAIT_PARALLELIMPORTEDUGRID_HPP
namespace Parfait{
  inline ParallelImportedUgrid::ParallelImportedUgrid() :
          ImportedUgrid({},{},{},{},{},{},{},{},{},{},{}){
  }

  inline ParallelImportedUgrid::ParallelImportedUgrid(long globalNodeCount_in,
                                                      std::vector<long> globalNodeIds_in,
                                                      std::vector<int> localNodeOwnershipDegree_in,
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
          globalNodeIds(globalNodeIds_in),
          localNodeOwnershipDegree(localNodeOwnershipDegree_in)
  {

      for(auto &degree : localNodeOwnershipDegree) {
          if (numberOfNodesAtDegree.count(degree) == 0)
              numberOfNodesAtDegree[degree] = 1;
          else
              numberOfNodesAtDegree[degree] = numberOfNodesAtDegree[degree]+1;
      }
  }
  inline long ParallelImportedUgrid::globalNumberOfNodes() const{
      return globalNodeCount;
  }
  inline int ParallelImportedUgrid::ownershipDegreeOfLocalNode(int localId) const {
      return localNodeOwnershipDegree[localId];
  }
  inline long ParallelImportedUgrid::getGlobalNodeId(int localId) const{
      return globalNodeIds[localId];
  }
  inline long ParallelImportedUgrid::numberOfNodesOfDegreeOrUnder(int degree) const{
      long count = 0;
      for(int i = 0; i <= degree; i++){
          if(numberOfNodesAtDegree.count(degree) == 0)
              count += 0;
          else
              count += numberOfNodesAtDegree.at(degree);
      }
      return count;
  }
  inline int ParallelImportedUgrid::getNodeComponentId(int localId) {
      return localNodeComponentIds[localId];
  }
}
#endif 
