#pragma once

#include <array>
#include "Point.h"
#include "Extent.h"

namespace Parfait {
  class Facet {

  public:
      Facet() = default;
      Facet(const Point<double> &p1, const Point<double> &p2, const Point<double> &p3);
      std::array<Point<double>, 3> points;
      Point<double> normal;

      Extent<double> getExtent() const;
      bool WhereDoesEdgeIntersect(const Point<double> &edgelo,
                                  const Point<double> &edgehi,
                                  Point<double> &point) const;

      bool DoesRayIntersect(const double orig[3], const double dir[3], double &t,
                            bool *confident = nullptr) const;

      Point<double> GetClosestPoint(Point<double> point) const;

      Point<double> &operator[](int i);
      const Point<double> &operator[](int i) const;

      int numberOfNodes() const { return 3; }

      void getNode(int i, double p[3]) const;
      Point<double> getNode(int i) const;

      double GetXMin();
      double GetYMin();
      double GetZMin();

      double GetXMax();
      double GetYMax();
      double GetZMax();
      Point<double> computeNormal() const;

      static double getLargestAngleBetween(const std::vector<Facet> &facets);

  private:
      double GetMax(int i);
      double GetMin(int i);
  };
}

#include "Facet.hpp"