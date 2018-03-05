
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
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

#include <parfait/MotionMatrix.h>
#include <parfait/MapbcReader.h>
#include <parfait/Point.h>
#include <MessagePasser/MessagePasser.h>


namespace Parfait {
  class Configuration {
  public:
      Configuration(int number_of_grids,
                    const std::vector<std::string>& grid_filenames,
                    const std::vector<bool>& is_big_endian,
                    const std::vector<Parfait::BoundaryConditionMap>& map_bcs,
                    const std::vector<Parfait::MotionMatrix>& motion_matrices);

      int numberOfGrids() const;
      std::string getFilename(int gridId) const;
      bool isBigEndian(int gridId) const;
      Parfait::MotionMatrix getMotionMatrix(int gridId) const;
      int getBoundaryCondition(int gridId, int tag) const;
      Parfait::BoundaryConditionMap getMapbcObject(int gridId) const;
      void print(MessagePasser mp) const;
  private:
      int ngrids;
      std::vector<std::string> gridFilenames;
      std::vector<bool> bigEndian;
      std::vector<Parfait::BoundaryConditionMap> mapbcVector;
      std::vector<Parfait::MotionMatrix> motionMatrices;
  };
}
#include "Configuration.hpp"
