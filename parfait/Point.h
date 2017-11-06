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
      Point operator*(const T a) const;
      Point operator/(const T a) const;
      Point operator*=(const T a);
      Point operator/=(const T a);
      Point operator+=(const Point &rhs);
      Point operator-=(const Point &rhs);
      bool operator==(const Point &rhs) const;
      bool operator!=(const Point &rhs) const;
      bool operator<(const Point &rhs) const;
      bool operator>(const Point &rhs) const;
      bool approxEqual(const Point &rhs, T tol = 1.0e-14) const;
      void normalize();
      T magnitude() const;
      template <typename U>
      friend Point<U> operator*(double a, const Point<U> &rhs);

      static T distance(const Point &a, const Point &b);
      static T dot(const Point &a, const Point &b);
      static T magnitude(const Point &a);
      static Point cross(const Point &a, const Point &b);
      static Point normalize(const Point &a);

      void Print() const;
  };
}

#include "Point.hpp"
