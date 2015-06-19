#include <stdio.h>
#include <iostream>
#include "extent.h"

namespace Parfait {
  template <typename T>
  Extent<T>::Extent() {
    lo = Point<T>(0, 0, 0);
    hi = Point<T>(0, 0, 0);
  }

  template <typename T>
  Extent<T>::Extent(const T extent[6]) {
    lo[0] = extent[0];
    lo[1] = extent[1];
    lo[2] = extent[2];

    hi[0] = extent[3];
    hi[1] = extent[4];
    hi[2] = extent[5];
  }

  template <typename T>
  Extent<T>::Extent(const Point<T> &lo_in, const Point<T> &hi_in)
      : lo(lo_in), hi(hi_in) { }

  template <typename T>
  bool Extent<T>::contains(const Point<T> &p) const {
    for (unsigned int i = 0; i < 3; i++) {
      if (p[i] < lo[i] || p[i] > hi[i]) {
        return false;
      }
    }
    return true;
  }

  template <typename T>
  bool Extent<T>::contains(const Extent<T> &box) const {
    for (unsigned int i = 0; i < 3; i++) {
      if (box.hi[i] < lo[i]) {
        return false;
      }
      if (box.lo[i] > hi[i]) {
        return false;
      }
    }
    return true;
  }

  template <typename T>
  T Extent<T>::getLength_X() const { return hi[0] - lo[0]; }

  template <typename T>
  T Extent<T>::getLength_Y() const { return hi[1] - lo[1]; }

  template <typename T>
  T Extent<T>::getLength_Z() const { return hi[2] - lo[2]; }

  template <typename T>
  void Extent<T>::Print() const {
    std::cout <<"lo: " << lo[0]<<" " << lo[1]<<" " << lo[2];
    std::cout <<", hi: " << hi[0]<<" " << hi[1]<<" " << hi[2] << std::endl;
  }

  template <typename T>
  Point<T> Extent<T>::center() const {
    return Point<T>(0.5 * (lo[0] + hi[0]), 0.5 * (lo[1] + hi[1]),
                 0.5 * (lo[2] + hi[2]));
  }

  template <typename T>
  void Extent<T>::resize(double scale) {
    scale *= 0.5;
    Point<T> centroid = center();
    double dx = scale * getLength_X();
    double dy = scale * getLength_Y();
    double dz = scale * getLength_Z();
    lo[0] = centroid[0] - dx;
    lo[1] = centroid[1] - dy;
    lo[2] = centroid[2] - dz;

    hi[0] = centroid[0] + dx;
    hi[1] = centroid[1] + dy;
    hi[2] = centroid[2] + dz;
  }

  template <typename T>
  void Extent<T>::resize(double scaleX, double scaleY, double scaleZ) {
    double dx = 0.5 * scaleX * getLength_X();
    double dy = 0.5 * scaleY * getLength_Y();
    double dz = 0.5 * scaleZ * getLength_Z();
    Point<T> centroid = center();
    lo[0] = centroid[0] - dx;
    lo[1] = centroid[1] - dy;
    lo[2] = centroid[2] - dz;

    hi[0] = centroid[0] + dx;
    hi[1] = centroid[1] + dy;
    hi[2] = centroid[2] + dz;
  }

  template<typename T>
  void Extent<T>::makeIsotropic() {
    T x = getLength_X();
    T y = getLength_Y();
    T z = getLength_Z();
    T largest = std::max(x,y);
    largest = std::max(largest,z);
    resize(largest/x,largest/y,largest/z);
  }

  template <typename T>
  Extent<T> Extent<T>::extentAroundPoint(const Point<T> &p, T tol) {
    Point<T> lo(p[0] - tol, p[1] - tol, p[2] - tol);
    Point<T> hi(p[0] + tol, p[1] + tol, p[2] + tol);
    return Extent(lo, hi);
  }

  template <typename T>
  double Extent<T>::volume(const Extent<T> &domain) {
    return domain.getLength_X() * domain.getLength_Y() * domain.getLength_Z();
  }
}