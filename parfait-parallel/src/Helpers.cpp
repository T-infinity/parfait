#include "Helpers.h"

namespace Helpers {
  Parfait::Point<double> computeTriangleArea(
          const Parfait::Point<double>& a,
          const Parfait::Point<double>& b,
          const Parfait::Point<double>& c) {
      auto v1 = b - a;
      auto v2 = c - a;
      return 0.5 * Point<double>::cross(v1, v2);
  }

  Parfait::Point<double> average(const std::vector<Parfait::Point<double>>& points) {
      Parfait::Point<double> avg = {0, 0, 0};
      for (auto& p : points)
          avg += p;
      return avg / (double) points.size();
  }

  Parfait::Point<double> computeCellCenter(const std::shared_ptr<Parfait::ParallelMesh>& mesh, int cell_id) {
      auto cell = mesh->getCell(cell_id);
      auto cell_length = mesh->numberOfNodesInCell(cell_id);
      Parfait::Point<double> center = {0, 0, 0};
      for (int i = 0; i < cell_length; i++) {
          center += mesh->getXyz(cell[i]);
      }
      return center / (double) cell_length;
  }
}
