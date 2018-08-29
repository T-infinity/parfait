#pragma once
#include <string>

namespace Parfait {
  template <typename T>
  std::string toReadableString(T value){
      std::string numWithCommas = std::to_string(value);
      int insertPosition = numWithCommas.length() - 3;
      while (insertPosition > 0) {
          numWithCommas.insert(insertPosition, ",");
          insertPosition-=3;
      }
      return numWithCommas;
  }
}


