#include <stdio.h>
#include "extent.h"

Extent::Extent() {
    lo = Point(0, 0, 0);
    hi = Point(0, 0, 0);
}

Extent::Extent(const double extent[6]) {
    lo[0] = extent[0];
    lo[1] = extent[1];
    lo[2] = extent[2];

    hi[0] = extent[3];
    hi[1] = extent[4];
    hi[2] = extent[5];
}

Extent::Extent(const Point &lo_in, const Point &hi_in) : lo(lo_in), hi(hi_in) {}

bool Extent::contains(const Point &p) const {
    for (unsigned int i = 0; i < 3; i++) {
        if (p[i] < lo[i] || p[i] > hi[i]) {
            return false;
        }
    }
    return true;
}

bool Extent::contains(const Extent &box) const {
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

double Extent::getLength_X() const { return hi[0] - lo[0]; }

double Extent::getLength_Y() const { return hi[1] - lo[1]; }

double Extent::getLength_Z() const { return hi[2] - lo[2]; }

void Extent::Print() const {
    printf("lo: %e %e %e", lo[0], lo[1], lo[2]);
    printf(", hi: %e %e %e", hi[0], hi[1], hi[2]);
}

Point Extent::center() const {
    return Point(0.5 * (lo[0] + hi[0]), 0.5 * (lo[1] + hi[1]),
            0.5 * (lo[2] + hi[2]));
}

void Extent::resize(double scale) {
    scale *= 0.5;
    Point centroid = center();
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

void Extent::resize(double scaleX, double scaleY, double scaleZ) {
    double dx = 0.5 * scaleZ * getLength_X();
    double dy = 0.5 * scaleY * getLength_Y();
    double dz = 0.5 * scaleZ * getLength_Z();
    Point centroid = center();
    lo[0] = centroid[0] - dx;
    lo[1] = centroid[1] - dy;
    lo[2] = centroid[2] - dz;

    hi[0] = centroid[0] + dx;
    hi[1] = centroid[1] + dy;
    hi[2] = centroid[2] + dz;
}

Extent Extent::extentAroundPoint(const Point &p, double tol) {
    Point lo(p[0] - tol, p[1] - tol, p[2] - tol);
    Point hi(p[0] + tol, p[1] + tol, p[2] + tol);
    return Extent(lo, hi);
}

double Extent::volume(const Extent &domain){
    return domain.getLength_X() * domain.getLength_Y() * domain.getLength_Z();
}
