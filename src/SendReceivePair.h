#pragma once
#include <vector>
#include <map>

namespace Parfait {
  struct SendReceivePair {
      std::map<int, std::vector<int>> send;
      std::vector<int> recv;
  };
}