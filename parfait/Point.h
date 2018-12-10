
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
#pragma once

#include <array>
#include <vector>

namespace Parfait {
  template <typename T>
  class Point {
  public:
      std::array<T,3> pos;

      Point() = default;
      Point(T x, T y, T z);
      Point(const T *);

      T *data() { return pos.data(); }

      const T *data() const { return pos.data(); }

      inline T &operator[](int i) {
        return pos[i];
      }

      inline const T &operator[](int i) const {
        return pos[i];
      }

      Point operator+(const Point &rhs) const;
      Point operator-(const Point &rhs) const;
      Point operator*(const T& a) const;
      Point operator/(const T& a) const;
      Point operator*=(const T& a);
      Point operator/=(const T& a);
      Point operator+=(const Point &rhs);
      Point operator-=(const Point &rhs);
      bool operator==(const Point &rhs) const;
      bool operator!=(const Point &rhs) const;
      bool operator<(const Point &rhs) const;
      bool operator>(const Point &rhs) const;
      bool approxEqual(const Point &rhs, T tol = 1.0e-14) const;
      void normalize();
      std::string to_string() const;
      T magnitude() const;
      template <typename U>
      friend Point<U> operator*(double a, const Point<U> &rhs);

      static T distance(const Point &a, const Point &b);
      static T dot(const Point &a, const Point &b);
      static T magnitude(const Point &a);
      static Point cross(const Point &a, const Point &b);
      static Point normalize(const Point &a);

  };
}

#include "Point.hpp"
