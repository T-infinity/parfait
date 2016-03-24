#pragma once
#include <stdexcept>
namespace Parfait {
  template<typename T>
  long getIndexOfItemInRange(const std::vector <T> &map, const T &item) {
      if (item > map.back() or item < map.front())
          throw std::logic_error("Searching for item (" + std::to_string(item) + ") out of range of compressed map "
                                 + std::to_string(map.front()) + " " + std::to_string(map.back()));
      auto it = std::lower_bound(map.begin(), map.end(), item);
      int index = it - map.begin();
      if (map[index] != item)
          index--;
      return index;
  }
}