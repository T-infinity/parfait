#ifndef EXTENT_H
#define EXTENT_H

#ifndef POINT_H
#include "point.h"
#endif

namespace Parfait {
  template <typename T>
  class Extent {

  public:
    Point<T> lo, hi;

    Extent();
    Extent(const T extent[6]);
    Extent(const Point<T> &lo, const Point<T> &hi);

    bool contains(const Point<T> &p) const;
    bool contains(const Extent &box) const;
    T getLength_X() const;
    T getLength_Y() const;
    T getLength_Z() const;
    Point<T> center() const;

    void resize(double scale);
    void resize(double scaleX, double scaleY, double scaleZ);

    void makeIsotropic();

    void Print() const;

    static Extent extentAroundPoint(const Point<T> &p, T tol);
    static double volume(const Extent &domain);
  };
}

#include <extent.hpp>

#endif
