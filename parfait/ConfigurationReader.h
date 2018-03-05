
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
#include <parfait/Configuration.h>
#include <parfait/MotionMatrix.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <parfait/MapbcReader.h>
#include <MessagePasser/MessagePasser.h>
#include <parfait/Point.h>
#include <nlohmann/json.hpp>
#include <fstream>

namespace Parfait {
  class ConfigurationReader {
  public:
      ConfigurationReader(std::string filename);
      Configuration createConfiguration();
  private:
      int ngrids;
      std::string filename;
      std::vector<std::string> gridFilenames;
      std::vector<bool> bigEndian;
      std::vector<Parfait::BoundaryConditionMap> mapbcVector;
      std::vector<Parfait::MotionMatrix> motionMatrices;
      void load();
      void readMapbcFiles();

      void throwIfConfigurationIsInsane();
      auto loadConfigFileAsJsonObject(std::string filename);
      bool getIsBigEndian(nlohmann::json& component);
      auto createMotionMatrixForComponent(nlohmann::json& component);
      auto getTranslation(nlohmann::json& component);
  };
}
#include "ConfigurationReader.hpp"
