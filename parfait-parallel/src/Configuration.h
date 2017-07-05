#pragma once
#include <string>
#include <vector>
#include <MotionMatrix.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <MapbcReader.h>
#include "Point.h"
#include "MessagePasser.h"
#include <memory>

namespace Parfait {
  class Configuration {
  public:
      Configuration(int number_of_grids,
                    const std::vector<std::string>& grid_filenames,
                    const std::vector<bool>& is_big_endian,
                    const std::vector<Parfait::BoundaryConditionMap>& map_bcs,
                    const std::vector<Parfait::MotionMatrix>& motion_matrices);

      int numberOfGrids() const;
      std::string getFilename(int gridId) const;
      bool isBigEndian(int gridId) const;
      Parfait::MotionMatrix getMotionMatrix(int gridId) const;
      int getBoundaryCondition(int gridId, int tag) const;
      Parfait::BoundaryConditionMap getMapbcObject(int gridId) const;
      void print(std::shared_ptr<MessagePasser> mp) const;
  private:
      int ngrids;
      std::vector<std::string> gridFilenames;
      std::vector<bool> bigEndian;
      std::vector<Parfait::BoundaryConditionMap> mapbcVector;
      std::vector<Parfait::MotionMatrix> motionMatrices;
  };
}
#include "Configuration.hpp"
