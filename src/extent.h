#ifndef EXTENT_H
#define EXTENT_H

#ifndef POINT_H
#include "point.h"
#endif

class Extent {

  public:
    Point lo, hi;

    Extent();
    Extent(const double extent[6]);
    Extent(const Point &lo, const Point &hi);

    bool contains(const Point &p) const;
    bool contains(const Extent &box) const;
    double getLength_X() const;
    double getLength_Y() const;
    double getLength_Z() const;
    Point center() const;

    void resize(double scale);
    void resize(double scaleX, double scaleY, double scaleZ);

    void Print() const;

    static Extent extentAroundPoint(const Point &p, double tol);
    static double volume(const Extent &domain);
};

#endif
