#pragma once
#include <string>
#include <vector>
#include <ConfigurationReader.h>
#include "Configuration.h"
#include <MotionMatrix.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <MapbcReader.h>
#include <MessagePasser.h>
#include "Point.h"
#include <json.hpp>
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
