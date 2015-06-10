#ifndef FACET_H
#define FACET_H

#include <array>

#ifndef POINT_H
#include "point.h"
#endif

#ifndef EXTENT_H
#include "extent.h"
#endif

namespace Parfait {
    class Facet {

    public:
        Facet() = default;
        Facet(const Point &p1, const Point &p2, const Point &p3);
        std::array<Point, 3> points;
        Point normal;

        Extent getExtent() const;
        bool WhereDoesEdgeIntersect(const Point &edgelo, const Point &edgehi, Point &point) const;

        bool DoesRayIntersect(const double orig[3], const double dir[3], double &t,
                              bool *confident = nullptr) const;

        Point GetClosestPoint(Point point, double &dist) const;

        Point &operator[](int i);
        const Point &operator[](int i) const;

        int numberOfNodes() const { return 3; }

        void getNode(int i, double p[3]) const;

        double GetXMin();
        double GetYMin();
        double GetZMin();

        double GetXMax();
        double GetYMax();
        double GetZMax();
        Point computeNormal();

        static double getLargestAngleBetween(const std::vector<Facet> &facets);

    private:
        double GetMax(int i);
        double GetMin(int i);
    };
}
#endif
