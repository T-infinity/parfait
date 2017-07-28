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