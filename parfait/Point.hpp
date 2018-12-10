
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
#include <assert.h>
#include <math.h>
#include <ostream>

namespace Parfait {

  template <typename T>
  Point<T>::Point(T a, T b, T c) {
    pos[0] = a;
    pos[1] = b;
    pos[2] = c;
  }


  template <typename T>
  Point<T>::Point(const T *p) {
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
  }

  template <typename T>
  Point<T> Point<T>::operator+(const Point<T> &rhs) const {
    return Point(
        pos[0] + rhs.pos[0],
        pos[1] + rhs.pos[1],
        pos[2] + rhs.pos[2]);
  }

  template <typename T>
  Point<T> Point<T>::operator-(const Point<T> &rhs) const {
    return Point<T>(
        pos[0] - rhs.pos[0],
        pos[1] - rhs.pos[1],
        pos[2] - rhs.pos[2]);
  }

  template <typename T>
  Point<T> Point<T>::operator*(const T& a) const {
    return Point<T>(
        a * pos[0],
        a * pos[1],
        a * pos[2]);
  }

  template <typename T>
  Point<T> Point<T>::operator/(const T& a) const {
    T ooa = 1.0 / a;
    return Point<T>(
        ooa * pos[0],
        ooa * pos[1],
        ooa * pos[2]);
  }

  template <typename T>
  Point<T> Point<T>::operator*=(const T& a) {
    pos[0] *= a;
    pos[1] *= a;
    pos[2] *= a;
    return *this;
  }

  template <typename T>
  Point<T> Point<T>::operator/=(const T& a) {
    T b = 1.0 / a;
    return *this *= b;
  }

  template <typename T>
  Point<T> operator*(double a, const Point<T> &rhs) {
    return Point<T>(
        a * rhs.pos[0],
        a * rhs.pos[1],
        a * rhs.pos[2]);
  }

  template <typename T>
  Point<T> Point<T>::operator+=(const Point<T> &rhs) {
    pos[0] += rhs.pos[0];
    pos[1] += rhs.pos[1];
    pos[2] += rhs.pos[2];
    return *this;
  }

  template <typename T>
  Point<T> Point<T>::operator-=(const Point<T> &rhs) {
    pos[0] -= rhs.pos[0];
    pos[1] -= rhs.pos[1];
    pos[2] -= rhs.pos[2];
    return *this;
  }

  template <typename T>
  bool Point<T>::operator==(const Point<T> &rhs) const{
    if (pos[0] != rhs.pos[0])
      return false;
    if (pos[1] != rhs.pos[1])
      return false;
    if (pos[2] != rhs.pos[2])
      return false;
    return true;
  }

  template <typename T>
  bool Point<T>::operator!=(const Point<T> &rhs) const {
      return not (*this == rhs);
  }

  template <typename T>
  bool Point<T>::operator<(const Point<T> &rhs) const {
      if (pos[0] < rhs.pos[0])
          return true;
      else if(pos[0] > rhs.pos[0])
          return false;
      if (pos[1] < rhs.pos[1])
          return true;
      else if(pos[1] < rhs.pos[1])
          return false;
      if (pos[2] < rhs.pos[2])
          return true;
      else if(pos[2] < rhs.pos[2])
          return false;
      return false;
  }

    template <typename T>
    bool Point<T>::operator>(const Point<T> &rhs) const {
        return (not (*this < rhs)) and (not (*this == rhs));
    }

  template <typename T>
  bool Point<T>::approxEqual(const Point<T> &rhs, T tol) const {
    if (pos[0] + tol < rhs.pos[0] or pos[0] - tol > rhs.pos[0])
      return false;
    if (pos[1] + tol < rhs.pos[1] or pos[1] - tol > rhs.pos[1])
      return false;
    if (pos[2] + tol < rhs.pos[2] or pos[2] - tol > rhs.pos[2])
      return false;
    return true;
  }

  template <typename T>
  T Point<T>::distance(const Point<T> &a, const Point<T> &b) {
    return sqrt(
        (b.pos[0] - a.pos[0]) * (b.pos[0] - a.pos[0]) +
        (b.pos[1] - a.pos[1]) * (b.pos[1] - a.pos[1]) +
        (b.pos[2] - a.pos[2]) * (b.pos[2] - a.pos[2]));
  }

  template <typename T>
  T Point<T>::dot(const Point<T> &a, const Point<T> &b) {
    return a.pos[0] * b.pos[0] + a.pos[1] * b.pos[1] + a.pos[2] * b.pos[2];
  }

  template <typename T>
  T Point<T>::magnitude() const {
    return sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
  }

  template <typename T>
  T Point<T>::magnitude(const Point<T> &a) {
    return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
  }

  template <typename T>
  Point<T> Point<T>::cross(const Point<T> &a, const Point<T> &b) {
    return Point(
        a[1] * b[2] - b[1] * a[2],
        b[0] * a[2] - a[0] * b[2],
        a[0] * b[1] - b[0] * a[1]
    );
  }

  template <typename T>
  void Point<T>::normalize() {
    T mag = magnitude(*this);
    mag += 1.0e-200;
    *this /= mag;
  }

  template <typename T>
  Point<T> Point<T>::normalize(const Point<T> &a) {
    Point<T> b{a};
    b.normalize();
    return b;
  }
  template<typename T>
  std::string Point<T>::to_string() const {
    std::string output = std::to_string(pos[0]) + " " +
        std::to_string(pos[1]) + " " +
        std::to_string(pos[2]);

    return output;
  }
}
