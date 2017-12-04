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

      bool DoesRayIntersect(const Parfait::Point<double>& orig,
                            const Parfait::Point<double>& dir,
                            double &t,
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

      bool intersects(const Extent<double> &e);
  private:
      double GetMax(int i);
      double GetMin(int i);
      bool triBoxOverlap(const Parfait::Point<double>&, const Parfait::Point<double>& boxhalfsize,
                         const std::array<Parfait::Point<double>, 3>& verts) const;
  };
}

#include "Facet.hpp"