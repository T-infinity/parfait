
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include <stdio.h>
#include <iostream>

namespace Parfait {
  template <typename T>
  Extent<T>::Extent() {
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
  bool Extent<T>::approxEqual(const Extent<T> &e) const{
    return lo.approxEqual(e.lo) and hi.approxEqual(e.hi);
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
    largest /= 2;
    auto c = center();
    lo = c - Point<T>{largest, largest, largest};
    hi = c + Point<T>{largest, largest, largest};
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

    template<typename T>
    std::array<Point<T>, 8>
    Extent<T>::corners() const {
      std::array<Point<T>, 8> points;

      points[0] = {lo[0], lo[1], lo[2]};
      points[1] = {hi[0], lo[1], lo[2]};
      points[2] = {hi[0], hi[1], lo[2]};
      points[3] = {lo[0], hi[1], lo[2]};

      points[4] = {lo[0], lo[1], hi[2]};
      points[5] = {hi[0], lo[1], hi[2]};
      points[6] = {hi[0], hi[1], hi[2]};
      points[7] = {lo[0], hi[1], hi[2]};

      return points;
    }
}