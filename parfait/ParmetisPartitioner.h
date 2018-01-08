#pragma once
#include "MessagePasser/MessagePasser.h"
#include <vector>
namespace Parfait {
  class ParmetisPartitioner {
  public:
      template<typename ContainerOfContainers>
      static std::vector<int> generatePartVector(MessagePasser mp,
                                          const ContainerOfContainers& node_to_node);
  };
}

#include "ParmetisPartitioner.hpp"