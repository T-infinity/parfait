#pragma once
#include <vector>

namespace Parfait {
  struct SendReceivePair {
      std::vector<std::vector<int>> send;
      std::vector<int> recv;
  };
}