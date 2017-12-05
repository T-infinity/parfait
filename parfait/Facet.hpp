#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <stdexcept>

#define TOL 1.0e-10

namespace LocalMacroCrap {
  inline double squaredMagnitude(const Parfait::Point<double> &a) {
      return a.pos[0] * a.pos[0] + a.pos[1] * a.pos[1] + a.pos[2] * a.pos[2];
  }

  inline void CROSS(double *dest, const double *v1, const double *v2) {
      dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
      dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
      dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
  }

  inline double DOT(const double *v1, const double *v2) {
      return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
  }

  inline void SUB(double *dest, const double *v1, const double *v2) {
      dest[0] = v1[0] - v2[0];
      dest[1] = v1[1] - v2[1];
      dest[2] = v1[2] - v2[2];
  }
}

inline std::pair<double, double> findMinMax(double a, double b, double c) {
    double min = a;
    double max = a;
    min = std::min(min, b);
    min = std::min(min, c);
    max = std::max(max, b);
    max = std::max(max, c);
    return std::make_pair(min, max);
};

inline Parfait::Facet::Facet(const Parfait::Point<double> &p1, const Parfait::Point<double> &p2, const Parfait::Point<double> &p3) {
    points = std::array<Parfait::Point<double>, 3> {p1, p2, p3};
}

inline Parfait::Point<double> &Parfait::Facet::operator[](int i) {
    if (i < 0 or i > 2)
        throw std::domain_error("Invalid access to facet.");
    return points[i];
}

inline const Parfait::Point<double> &Parfait::Facet::operator[](int i) const {
    if (i < 0 or i > 2)
        throw std::domain_error("Invalid access to facet.");
    return points[i];
}

inline double Parfait::Facet::GetMin(int i) {
    double t = points[0][i];
    t = (t < points[1][i]) ? (t) : (points[1][i]);
    t = (t < points[2][i]) ? (t) : (points[2][i]);
    return t;
}

inline double Parfait::Facet::GetMax(int i) {
    double t = points[0][i];
    t = (t > points[1][i]) ? (t) : (points[1][i]);
    t = (t > points[2][i]) ? (t) : (points[2][i]);
    return t;
}

inline double Parfait::Facet::GetXMin() {
    return GetMin(0);
}

inline double Parfait::Facet::GetYMin() {
    return GetMin(1);
}

inline double Parfait::Facet::GetZMin() {
    return GetMin(2);
}

inline double Parfait::Facet::GetXMax() {
    return GetMax(0);
}

inline double Parfait::Facet::GetYMax() {
    return GetMax(1);
}

inline double Parfait::Facet::GetZMax() {
    return GetMax(2);
}

inline bool Parfait::Facet::DoesRayIntersect(const Parfait::Point<double>& orig,
                                             const Parfait::Point<double>& dir,
                                             double &t, bool *confident) const {


    if (confident != nullptr) {
        *confident = true;
    }

    auto edge1 = points[1] - points[0];
    auto edge2 = points[2] - points[0];

    auto pvec = Parfait::Point<double>::cross(dir, edge2);
    double det = Parfait::Point<double>::dot(edge1, pvec);

    if (fabs(det) < 1.0e-15) {
        return false;
    }

    double inv_det = 1.0 / det;
    auto tvec = orig - points[0];

    double u = Parfait::Point<double>::dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    auto qvec = Parfait::Point<double>::cross(tvec, edge1);
    double v = Parfait::Point<double>::dot(dir, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    t = Parfait::Point<double>::dot(edge2, qvec) * inv_det;
    if (t < 0.0) {
        return false;
    }

    if (confident == nullptr) {
        return true;
    }

    if (v < 1.0e-12 || v > 1.0 + 1.0e-12 || u < 1.0e-12 || u > 1.0 + 1.0e-12) {
        *confident = false;
    } else {
        *confident = true;
    }
    return true;
}

inline bool Parfait::Facet::WhereDoesEdgeIntersect(const Point<double> &edgelo,
                                                   const Point<double> &edgehi,
                                                   Point<double> &point) const {

    double dir[3];
    double dist;
    dir[0] = edgehi[0] - edgelo[0];
    dir[1] = edgehi[1] - edgelo[1];
    dir[2] = edgehi[2] - edgelo[2];
    dist = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);

    double oodist = 1.0 / dist;
    dir[0] *= oodist;
    dir[1] *= oodist;
    dir[2] *= oodist;

    double t;
    if (!DoesRayIntersect(&edgelo[0], dir, t)) {
        return false;
    }
    if (t > dist) {
        return false;
    }

    point[0] = edgelo[0] + dir[0] * t;
    point[1] = edgelo[1] + dir[1] * t;
    point[2] = edgelo[2] + dir[2] * t;
    return true;
}

inline Parfait::Extent<double> Parfait::Facet::getExtent() const {

    Extent<double> extent;
    double x, y, z;
    x = (points[0][0] < points[1][0]) ? (points[0][0]) : (points[1][0]);
    x = (x < points[2][0]) ? (x) : (points[2][0]);
    y = (points[0][1] < points[1][1]) ? (points[0][1]) : (points[1][1]);
    y = (y < points[2][1]) ? (y) : (points[2][1]);
    z = (points[0][2] < points[1][2]) ? (points[0][2]) : (points[1][2]);
    z = (z < points[2][2]) ? (z) : (points[2][2]);

    extent.lo[0] = x;
    extent.lo[1] = y;
    extent.lo[2] = z;

    x = (points[0][0] > points[1][0]) ? (points[0][0]) : (points[1][0]);
    x = (x > points[2][0]) ? (x) : (points[2][0]);
    y = (points[0][1] > points[1][1]) ? (points[0][1]) : (points[1][1]);
    y = (y > points[2][1]) ? (y) : (points[2][1]);
    z = (points[0][2] > points[1][2]) ? (points[0][2]) : (points[1][2]);
    z = (z > points[2][2]) ? (z) : (points[2][2]);

    extent.hi[0] = x;
    extent.hi[1] = y;
    extent.hi[2] = z;

    return extent;
}

inline void Parfait::Facet::getNode(int i, double p[3]) const {
    switch (i) {
        case 0:p[0] = points[0][0];
            p[1] = points[0][1];
            p[2] = points[0][2];
            return;
        case 1:p[0] = points[1][0];
            p[1] = points[1][1];
            p[2] = points[1][2];
            return;
        case 2:p[0] = points[2][0];
            p[1] = points[2][1];
            p[2] = points[2][2];
            return;
    }
}

inline Parfait::Point<double> Parfait::Facet::getNode(int i) const {
    Point<double> p;
    getNode(i, p.data());
    return p;
}

inline Parfait::Point<double> Parfait::Facet::GetClosestPoint(Point<double> safe) const {

    Point<double> point = safe;
    Point<double> diff = points[0] - point;
    const Point<double> edge0 = points[1] - points[0];
    const Point<double> edge1 = points[2] - points[0];

    double a00 = LocalMacroCrap::squaredMagnitude(edge0);
    double a01 = Point<double>::dot(edge0, edge1);
    double a11 = LocalMacroCrap::squaredMagnitude(edge1);
    double b0 = Point<double>::dot(diff, edge0);
    double b1 = Point<double>::dot(diff, edge1);
    double det = fabs(a00 * a11 - a01 * a01);
    double s = a01 * b1 - a11 * b0;
    double t = a01 * b0 - a00 * b1;
    /*
      \     |
       \reg2|
        \   |
         \  |
          \ |
           \|
            *points[1]
            |\
            | \
      reg3  |  \ reg1
            |   \
            |reg0\
            |     \
            |      \ points[0]
     -------*-------*------->s
            |P0      \
      reg4  | reg5    \ reg6
    */
    if (s + t <= det) {
        if (s < (double) 0) {
            if (t < (double) 0) {
                if (b0 < (double) 0) {
                    t = (double) 0;
                    if (-b0 >= a00)
                        s = (double) 1;
                    else
                        s = -b0 / a00;
                } else {
                    s = (double) 0;
                    if (b1 >= (double) 0)
                        t = (double) 0;
                    else if (-b1 >= a11)
                        t = (double) 1;
                    else
                        t = -b1 / a11;
                }
            } else {
                s = (double) 0;
                if (b1 >= (double) 0)
                    t = (double) 0;
                else if (-b1 >= a11)
                    t = (double) 1;
                else
                    t = -b1 / a11;
            }
        } else if (t < (double) 0) {
            t = (double) 0;
            if (b0 >= (double) 0)
                s = (double) 0;
            else if (-b0 >= a00)
                s = (double) 1;
            else
                s = -b0 / a00;
        } else {
            double invDet = ((double) 1) / det;
            s *= invDet;
            t *= invDet;
        }
    } else {
        double tmp0, tmpoints, numer, denom;

        if (s < (double) 0) {
            tmp0 = a01 + b0;
            tmpoints = a11 + b1;
            if (tmpoints > tmp0) {
                numer = tmpoints - tmp0;
                denom = a00 - ((double) 2) * a01 + a11;
                if (numer >= denom) {
                    s = (double) 1;
                    t = (double) 0;
                } else {
                    s = numer / denom;
                    t = (double) 1 - s;
                }
            } else {
                s = (double) 0;
                if (tmpoints <= (double) 0)
                    t = (double) 1;
                else if (b1 >= (double) 0)
                    t = (double) 0;
                else
                    t = -b1 / a11;
            }
        } else if (t < (double) 0) {
            tmp0 = a01 + b1;
            tmpoints = a00 + b0;
            if (tmpoints > tmp0) {
                numer = tmpoints - tmp0;
                denom = a00 - ((double) 2) * a01 + a11;
                if (numer >= denom) {
                    t = (double) 1;
                    s = (double) 0;
                } else {
                    t = numer / denom;
                    s = (double) 1 - t;
                }
            } else {
                t = (double) 0;
                if (tmpoints <= (double) 0)
                    s = (double) 1;
                else if (b0 >= (double) 0)
                    s = (double) 0;
                else
                    s = -b0 / a00;
            }
        } else {
            numer = a11 + b1 - a01 - b0;
            if (numer <= (double) 0) {
                s = (double) 0;
                t = (double) 1;
            } else {
                denom = a00 - ((double) 2) * a01 + a11;
                if (numer >= denom) {
                    s = (double) 1;
                    t = (double) 0;
                } else {
                    s = numer / denom;
                    t = (double) 1 - s;
                }
            }
        }
    }

    point = points[0] + s * edge0 + t * edge1;
    return point;
}

inline Parfait::Point<double> Parfait::Facet::computeNormal() const {
    Point<double> a = points[1] - points[0];
    Point<double> b = points[2] - points[0];

    Point<double> c = Point<double>::cross(a, b);
    c.normalize();
    return c;
}

inline double Parfait::Facet::getLargestAngleBetween(const std::vector<Facet> &facets) {
    double min = 10e20;
    for (auto fi : facets) {
        auto ui = fi.computeNormal();
        for (auto fj : facets) {
            auto uj = fj.computeNormal();
            double dot = fabs(Point<double>::dot(ui, uj));
            min = std::min(dot, min);
        }
    }
    return acos(min) * 180 / 3.141592653; //close enough to pi
}

inline bool Parfait::Facet::intersects(const Parfait::Extent<double> &e) const {

    Parfait::Point<double> center = 0.5*(e.hi + e.lo);
    Parfait::Point<double> halfs = 0.5*(e.hi - e.lo);

    return triBoxOverlap(center, halfs, this->points);
}

inline bool planeBoxOverlap(const Parfait::Point<double> &normal, const Parfait::Point<double> &vert,
                     const Parfait::Point<double> &maxbox) {
    Parfait::Point<double> vmin, vmax;
    for (int q = 0; q < 3; q++) {
        double v = vert[q];
        if (normal[q] > 0.0) { vmin[q] = -maxbox[q] - v;
            vmax[q] = maxbox[q] - v;
        } else {
            vmin[q] = maxbox[q] - v;
            vmax[q] = -maxbox[q] - v;
        }
    }
    if (Parfait::Point<double>::dot(normal, vmin) - TOL > 0.0) {
        return false;
    }
    if (Parfait::Point<double>::dot(normal, vmax) + TOL >= 0.0) {
        return true;
    }

    return false;
}

inline bool AXISTEST_X(double a, double b, double fa, double fb,
                         const Parfait::Point<double>& v0,
                         const Parfait::Point<double>& v2,
                         const Parfait::Point<double>& boxhalfsize) {
    double p0 = a * v0[1] - b * v0[2];
    double p2 = a * v2[1] - b * v2[2];
    double min, max;
    if (p0 < p2) {
        min = p0;
        max = p2;
    } else {
        min = p2;
        max = p0;
    }
    double rad = fa * boxhalfsize[1] + fb * boxhalfsize[2];
    return (min - TOL > rad || max + TOL < -rad);
}

inline bool AXISTEST_Y(double a, double b, double fa, double fb,
                         const Parfait::Point<double>& v0,
                         const Parfait::Point<double>& v2,
                         const Parfait::Point<double>& boxhalfsize) {
    double p0 = -a * v0[0] + b * v0[2];
    double p2 = -a * v2[0] + b * v2[2];
    double min, max;
    if (p0 < p2) {
        min = p0;
        max = p2;
    } else {
        min = p2;
        max = p0;
    }
    double rad = fa * boxhalfsize[0] + fb * boxhalfsize[2];
    return (min - TOL > rad || max + TOL < -rad);
}

inline bool AXISTEST_Z(double a, double b, double fa, double fb,
                         const Parfait::Point<double>& v1,
                         const Parfait::Point<double>& v2,
                         const Parfait::Point<double>& boxhalfsize) {
    double p1 = a * v1[0] - b * v1[1];
    double p2 = a * v2[0] - b * v2[1];
    double min, max;
    if (p2 < p1) {
        min = p2;
        max = p1;
    } else {
        min = p1;
        max = p2;
    }
    double rad = fa * boxhalfsize[0] + fb * boxhalfsize[1];
    return (min - TOL > rad || max + TOL < -rad);
}

inline bool Parfait::Facet::triBoxOverlap(
    const Parfait::Point<double>& boxcenter,
    const Parfait::Point<double>& boxhalfsize,
    const std::array<Parfait::Point<double>,3>& triverts) const {

    auto v0 = triverts[0] - boxcenter;
    auto v1 = triverts[1] - boxcenter;
    auto v2 = triverts[2] - boxcenter;

    auto e0 = v1 - v0;
    auto e1 = v2 - v1;
    auto e2 = v0 - v2;

    auto fex = fabs(e0[0]);
    auto fey = fabs(e0[1]);
    auto fez = fabs(e0[2]);
    if(AXISTEST_X(e0[2], e0[1], fez, fey, v0, v2, boxhalfsize)) return false;
    if(AXISTEST_Y(e0[2], e0[0], fez, fex, v0, v2, boxhalfsize)) return false;
    if(AXISTEST_Z(e0[1], e0[0], fey, fex, v1, v2, boxhalfsize)) return false;

    fex = fabs(e1[0]);
    fey = fabs(e1[1]);
    fez = fabs(e1[2]);
    if(AXISTEST_X(e1[2], e1[1], fez, fey, v0, v2, boxhalfsize)) return false;
    if(AXISTEST_Y(e1[2], e1[0], fez, fex, v0, v2, boxhalfsize)) return false;
    if(AXISTEST_Z(e1[1], e1[0], fey, fex, v0, v1, boxhalfsize)) return false;

    fex = fabs(e2[0]);
    fey = fabs(e2[1]);
    fez = fabs(e2[2]);
    if(AXISTEST_X(e2[2], e2[1], fez, fey, v0, v1, boxhalfsize)) return false;
    if(AXISTEST_Y(e2[2], e2[0], fez, fex, v0, v1, boxhalfsize)) return false;
    if(AXISTEST_Z(e2[1], e2[0], fey, fex, v1, v2, boxhalfsize)) return false;

    for(int i = 0; i < 3; i++) {
        double min, max;
        std::tie(min, max) = findMinMax(v0[i], v1[i], v2[i]);
        if (min - TOL > boxhalfsize[i] || max + TOL < -boxhalfsize[i]) return false;
    }
    auto normal = Parfait::Point<double>::cross(e0, e1);
    return planeBoxOverlap(normal, v0, boxhalfsize);
}
