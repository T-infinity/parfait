
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
#include "ParallelMeshReader.h"
#include "ParallelNodeToNodeBuilder.h"
#include <thread>
#include "NodeCenteredRedistributor.h"
#include "Timing.h"
#include "SurfaceNeighbors.h"
#include "SurfaceElementWinding.h"
#include "ParmetisPartitioner.h"

namespace Parfait {
  inline PreProcessor::PreProcessor(MessagePasser mp,
                                    Configuration& configuration)
          : mp(mp), config(configuration) {}

  inline std::shared_ptr<ParallelMesh> PreProcessor::createMesh() {
      auto before_reading = Now();
      std::vector<std::string> grid_names;
      std::vector<bool> is_big_endian;
      for (int i = 0; i < config.numberOfGrids(); i++) {
          grid_names.push_back(config.getFilename(i));
          is_big_endian.push_back(config.isBigEndian(i));
      }
      auto mesh = getDistributedMesh(grid_names, is_big_endian);
      auto after_reading = Now();
      Parfait::ParallelNodeToNodeBuilder<Parfait::ParallelMesh> n2n_builder(*mesh.get());
      auto n2n = n2n_builder.buildNodeToNodeConnectivity();
      auto after_building_node_to_node = Now();

      auto part = ParmetisPartitioner::generatePartVector(mp, n2n);
      auto after_parmetis = Now();
      NodeBasedRedistributor distributor(mp, mesh, part);
      auto distributed = distributor.redistribute();
      auto after_redistributing = Now();

      auto surface_neighbors = SurfaceNeighbors::buildSurfaceNeighbors(distributed);
      windAllSurfaceElementsOut(mp, distributed, surface_neighbors);
      if (not areSurfaceElementsWoundOut(distributed, surface_neighbors))
          throw std::logic_error("\nSurface elements are not all wound out");

      return distributed;
  }

  inline std::shared_ptr<ParallelMesh> PreProcessor::getDistributedMesh(std::vector<std::string>& grid_names,
                                                                        std::vector<bool>& is_big_endian) {
      auto reader = std::make_shared<Parfait::ParallelMeshReader>(mp, grid_names, is_big_endian);
      return reader->distributeGridsEvenly();
  }
}
