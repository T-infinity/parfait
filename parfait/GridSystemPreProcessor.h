
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
#include <iostream>
#include "ConfigurationReader.h"
#include "ParallelMesh.h"
#include "PreProcessor.h"

namespace Parfait {

  inline std::vector<std::shared_ptr<Parfait::ParallelMesh>> importMeshesAsSystem(
          MessagePasser mp,
          std::string filename) {
      ConfigurationReader reader(filename);
      auto config = reader.createConfiguration();

      std::vector<Parfait::Configuration> system_configs;
      for (int g = 0; g < config.numberOfGrids(); g++) {
          auto file_names = std::vector<std::string>();
          file_names.push_back(config.getFilename(g));

          auto is_big_endian = std::vector<bool>();
          is_big_endian.push_back(config.isBigEndian(g));

          auto map_bcs = std::vector<Parfait::BoundaryConditionMap>();
          map_bcs.push_back(config.getMapbcObject(g));

          auto motion_matrices = std::vector<Parfait::MotionMatrix>();
          motion_matrices.push_back(config.getMotionMatrix(g));

          system_configs.push_back({1, file_names, is_big_endian, map_bcs, motion_matrices});
      }

      std::vector<std::shared_ptr<Parfait::ParallelMesh>> grid_system;
      for (auto c : system_configs) {
          PreProcessor preProcessor(mp, c);
          auto g = preProcessor.createMesh();
          grid_system.push_back(g);
      }
      return grid_system;
  }
}
