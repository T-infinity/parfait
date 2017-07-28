#pragma once

#include "ImportedUgrid.h"
namespace Parfait {
  class ParallelImportedUgrid : public ImportedUgrid {
  public:
      ParallelImportedUgrid();
      ParallelImportedUgrid(long globalNodeCount, const std::vector<long>& globalNodeIds,
                            const std::vector<int>& localNodeOwnershipDegree,
                            const std::vector<int>& localNodeComponentIds,
                            const std::vector<double>& nodes,
                            const std::vector<int>& triangles,
                            const std::vector<int>& quads,
                            const std::vector<int>& tets,
                            const std::vector<int>& pyramids,
                            const std::vector<int>& prisms,
                            const std::vector<int>& hexs,
                            const std::vector<int>& triangleTags,
                            const std::vector<int>& quadTags);
      long globalNumberOfNodes() const;
      long getGlobalNodeId(int localId) const;
      int ownershipDegreeOfLocalNode(int localId) const;
      long numberOfNodesOfDegreeOrUnder(int degree) const;
      int getNodeComponentId(int localId);

  public:
      long globalNodeCount = 0;
      std::vector<long> globalNodeIds;
      std::vector<int> localNodeOwnershipDegree;
      std::map<int, long> numberOfNodesAtDegree;
      std::vector<int> localNodeComponentIds;
  };
}

#include "ParallelImportedUgrid.hpp"