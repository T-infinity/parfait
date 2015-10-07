#pragma once

namespace Parfait {
  template<typename T>
  long getIndexOfItemInRange(const std::vector <T> &map, const T &item) {
      if (item >= map.back() or item < map.front())
          throw std::logic_error("Searching for item out of range of compressed map");
      auto it = std::lower_bound(map.begin(), map.end(), item);
      int index = it - map.begin();
      if (map[index] != item)
          index--;
      return index;
  }
}