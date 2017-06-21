#pragma once
#include <string>
#include "ConfigurationReader.h"
#include "Configuration.h"
#include <MessagePasser.h>
#include <memory>
#include "ParallelMesh.h"

namespace Parfait {
  class PreProcessor {

  public:
      PreProcessor(std::shared_ptr<MessagePasser> mp,
                   Configuration& configuration);
      std::shared_ptr<ParallelMesh> createMesh();

      int getNumberOfGrids() { return config.numberOfGrids(); }

      std::string getGridName(int i) { return config.getFilename(i); }

      bool getIsBigEndian(int i) { return config.isBigEndian(i); }

  private:
      std::shared_ptr<MessagePasser> mp;
      Configuration& config;
      std::vector<int> calculateNewPartitioning(std::shared_ptr<ParallelMesh> mesh);
      std::shared_ptr<ParallelMesh> getDistributedMesh(std::vector<std::string>& grid_names,
                                                       std::vector<bool>& is_big_endian);
  };
}
#include "PreProcessor.hpp"