#pragma once
#include <vector>
#include "Partitioner.h"

namespace Parfait {
  class ParmetisPartitioner : public Partitioner {
  public:
      std::vector<int> generatePartVector(const std::vector<std::vector<long>> &node_to_node);
  };
}

#include "ParmetisPartitioner.hpp"