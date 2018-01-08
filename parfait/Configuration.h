#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

#include <parfait/MotionMatrix.h>
#include <parfait/MapbcReader.h>
#include <parfait/Point.h>
#include <MessagePasser/MessagePasser.h>


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
      void print(MessagePasser mp) const;
  private:
      int ngrids;
      std::vector<std::string> gridFilenames;
      std::vector<bool> bigEndian;
      std::vector<Parfait::BoundaryConditionMap> mapbcVector;
      std::vector<Parfait::MotionMatrix> motionMatrices;
  };
}
#include "Configuration.hpp"
