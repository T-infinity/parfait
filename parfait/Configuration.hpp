
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
#include <vector>
#include <string>
#include <MessagePasser/MessagePasser.h>

namespace Parfait {
  inline Configuration::Configuration(int number_of_grids, const std::vector<std::string>& grid_filenames,
                                      const std::vector<bool>& is_big_endian,
                                      const std::vector<Parfait::BoundaryConditionMap>& map_bcs,
                                      const std::vector<Parfait::MotionMatrix>& motion_matrices)
          : ngrids(number_of_grids),
            gridFilenames(grid_filenames),
            bigEndian(is_big_endian),
            mapbcVector(map_bcs),
            motionMatrices(motion_matrices) {}

  inline int Configuration::numberOfGrids() const { return ngrids; }

  inline bool Configuration::isBigEndian(int gridId) const { return bigEndian.at(gridId); }

  inline std::string Configuration::getFilename(int gridId) const { return gridFilenames.at(gridId); }

  inline Parfait::MotionMatrix Configuration::getMotionMatrix(int gridId) const { return motionMatrices.at(gridId); }

  inline Parfait::BoundaryConditionMap Configuration::getMapbcObject(int gridId) const {
      return mapbcVector.at(gridId);
  }

  inline int Configuration::getBoundaryCondition(int gridId, int tag) const {
      return mapbcVector.at(gridId).at(gridId).first;
  }

  inline void Configuration::print(MessagePasser mp) const {
      if (0 == mp.Rank()) {
          for (unsigned int i = 0; i < gridFilenames.size(); i++) {
              printf("Grid %i:\n", i);
              printf("  -file        %s\n", gridFilenames.at(i).c_str());
              if (bigEndian.at(i))
                  printf("  -endianness  big\n");
              else
                  printf("  -endianness  little\n");
              printf("  -Motion matrix:");
              auto m = motionMatrices.at(i).toString();
              printf("%s",m.c_str());
          }
      }
  }
}
