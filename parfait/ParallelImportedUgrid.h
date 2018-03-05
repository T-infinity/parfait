
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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