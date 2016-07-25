#pragma once
#include "Adt.h"
#include "Point.h"
#include "Extent.h"
#include <string>
#include <stdexcept>

namespace Parfait {
  class Adt3DPoint {
  public:
      Adt3DPoint(const Extent<double> &domain);
      void store(int tag, const Point<double> &point);
      std::vector<int> retrieve(const Extent<double> &domain) const;
      void removeFirst(int tag, const Extent<double>& domain);

      bool isPointInAdt(const Point<double> &p, int &tag, double tol = 1.0e-8) const;
      int getNodeIdOrAddPoint(const Point<double>& p, int tag);
      int getNodeId(const Point<double>& p);

  private:
      UnitTransformer unitTransformer;
      Adt<3> adt;
  };
}

#include "Adt3dPoint.hpp"
