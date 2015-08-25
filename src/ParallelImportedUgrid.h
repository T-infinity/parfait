#ifndef PARFAIT_PARALLELIMPORTEDUGRID_H
#define PARFAIT_PARALLELIMPORTEDUGRID_H

#include <ImportedUgrid.h>
namespace Parfait {
  class ParallelImportedUgrid : public ImportedUgrid {
  public:
      ParallelImportedUgrid();
      ParallelImportedUgrid(long globalNodeCount, std::vector<long> globalNodeIds,
                            std::vector<int> localNodeOwnershipDegree,
                            std::vector<int> localNodeComponentIds,
                            std::vector<double> nodes, std::vector<int> triangles,
                            std::vector<int> quads, std::vector<int> tets, std::vector<int> pyramids,
                            std::vector<int> prisms, std::vector<int> hexs,
                            std::vector<int> triangleTags, std::vector<int> quadTags,
                            std::vector<int> triangleBoundaryConditions,
                            std::vector<int> quadBoundaryConditions);
      long globalNumberOfNodes() const;
      long getGlobalNodeId(int localId) const;
      int ownershipDegreeOfLocalNode(int localId) const;
      long numberOfNodesOfDegreeOrUnder(int degree) const;
      int getNodeComponentId(int localId);

  protected:
      long globalNodeCount = 0;
      std::vector<long> globalNodeIds;
      std::vector<int> localNodeOwnershipDegree;
      std::map<int, long> numberOfNodesAtDegree;
      std::vector<int> localNodeComponentIds;
  };

}

#include "ParallelImportedUgrid.hpp"

#endif 
