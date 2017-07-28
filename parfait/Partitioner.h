#pragma once
#include <vector>
#include <memory>
#include "ParallelMesh.h"

namespace Parfait {
  class Partitioner {
  public:
      virtual std::vector<int> generatePartVector(
              std::shared_ptr<MessagePasser> mp,
              const std::vector<std::vector<long>>& node_to_node) = 0;
  };

  class ErrorPartitioner : public Partitioner {
      virtual std::vector<int> generatePartVector(const std::vector<std::vector<long>>& node_to_node) {
          throw std::logic_error("No partitioners available");
      }
  };
}