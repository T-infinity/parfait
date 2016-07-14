#pragma once
#include <string>

namespace Parfait {
  template<class Mesh>
  class VtkExplodedViewWriter {
  public:
      VtkExplodedViewWriter(const Mesh &mesh, std::string filename);
      void writeAscii();
  private:
      const Mesh &mesh;
      std::string filename;
  };
}
#include "VtkExplodedViewWriter.hpp"