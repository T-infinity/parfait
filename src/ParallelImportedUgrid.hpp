#ifndef PARFAIT_PARALLELIMPORTEDUGRID_HPP
#define PARFAIT_PARALLELIMPORTEDUGRID_HPP
namespace Parfait{
  inline ParallelImportedUgrid::ParallelImportedUgrid() :
          ImportedUgrid({},{},{},{},{},{},{},{},{}){
  }

  inline ParallelImportedUgrid::ParallelImportedUgrid(long globalNodeCount_in,
                                                      const std::vector<long>& globalNodeIds_in,
                                                      const std::vector<int>& localNodeOwnershipDegree_in,
                                                      const std::vector<int>& localNodeComponentIds_in,
                                                      const std::vector<double>& nodes,
                                                      const std::vector<int>& triangles,
                                                      const std::vector<int>& quads,
                                                      const std::vector<int>& tets,
                                                      const std::vector<int>& pyramids,
                                                      const std::vector<int>& prisms,
                                                      const std::vector<int>& hexs,
                                                      const std::vector<int>& triangleTags,
                                                      const std::vector<int>& quadTags) :
          ImportedUgrid(nodes, triangles, quads, tets, pyramids, prisms, hexs, triangleTags, quadTags),
          globalNodeCount(globalNodeCount_in),
          globalNodeIds(globalNodeIds_in),
          localNodeOwnershipDegree(localNodeOwnershipDegree_in),
          localNodeComponentIds(localNodeComponentIds_in)
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
