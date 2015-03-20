#ifndef UNIT_TRANSFORMER_H
#define UNIT_TRANSFORMER_H

#ifndef POINT_H
#include "point.h"
#endif

#ifndef EXTENT_H
#include "extent.h"
#endif

class UnitTransformer {

    public:
    UnitTransformer(const Extent &extent_i) : extent(extent_i) { GetScale(); }

    inline Extent getDomain() const { return extent;}

    inline double GetScale() {
        double dx = extent.hi[0] - extent.lo[0];
        double dy = extent.hi[1] - extent.lo[1];
        double dz = extent.hi[2] - extent.lo[2];
        double ddum = (dx > dy) ? (dx) : (dy);
        scale = (ddum > dz) ? (ddum) : (dz);
        ooscale = 1.0 / scale;
        return scale;
    }

    inline Point ToUnitSpace(const Point &physicalPoint) const {
        Point temp(physicalPoint);
        temp[0] -= extent.lo[0];
        temp[1] -= extent.lo[1];
        temp[2] -= extent.lo[2];

        temp[0] *= ooscale;
        temp[1] *= ooscale;
        temp[2] *= ooscale;

        return temp;
    }

    inline Point ToPhysicalSpace(const Point &unitPoint) const {
        Point temp(unitPoint);
        temp[0] *= scale;
        temp[1] *= scale;
        temp[2] *= scale;

        temp[0] += extent.lo[0];
        temp[1] += extent.lo[1];
        temp[2] += extent.lo[2];

        return temp;
    }

    private:
    Extent extent;
    double scale;
    double ooscale;  // One over scale;
};

#endif
