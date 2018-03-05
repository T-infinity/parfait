
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
#include <memory>
#include <string>
#include <parfait/ConfigurationReader.h>
#include <parfait/Configuration.h>
#include <MessagePasser/MessagePasser.h>

#include <parfait/ParallelMesh.h>

namespace Parfait {
  class PreProcessor {

  public:
      PreProcessor(MessagePasser mp,
                   Configuration& configuration);
      std::shared_ptr<ParallelMesh> createMesh();

      int getNumberOfGrids() { return config.numberOfGrids(); }

      std::string getGridName(int i) { return config.getFilename(i); }

      bool getIsBigEndian(int i) { return config.isBigEndian(i); }

  private:
      MessagePasser mp;
      Configuration& config;
      std::vector<int> calculateNewPartitioning(std::shared_ptr<ParallelMesh> mesh);
      std::shared_ptr<ParallelMesh> getDistributedMesh(std::vector<std::string>& grid_names,
                                                       std::vector<bool>& is_big_endian);
  };
}
#include "PreProcessor.hpp"