#ifndef UNIT_TRANSFORMER_H
#define UNIT_TRANSFORMER_H

#ifndef POINT_H
#include "Point.h"
#endif

#ifndef EXTENT_H
#include "Extent.h"
#endif

namespace Parfait {
    class UnitTransformer {

    public:
        UnitTransformer(const Extent<double> &extent_i)
            : extent(extent_i) { GetScale(); }

        inline Extent<double> getDomain() const { return extent; }

        inline double GetScale() {
            double dx = extent.hi[0] - extent.lo[0];
            double dy = extent.hi[1] - extent.lo[1];
            double dz = extent.hi[2] - extent.lo[2];
            double ddum = (dx > dy) ? (dx) : (dy);
            scale = (ddum > dz) ? (ddum) : (dz);
            oneOverScale = 1.0 / scale;
            return scale;
        }

        inline Point<double> ToUnitSpace(const Point<double> &physicalPoint) const {
            Point<double> temp(physicalPoint);
            temp[0] -= extent.lo[0];
            temp[1] -= extent.lo[1];
            temp[2] -= extent.lo[2];

            temp[0] *= oneOverScale;
            temp[1] *= oneOverScale;
            temp[2] *= oneOverScale;

            return temp;
        }

        inline Point<double> ToPhysicalSpace(const Point<double> &unitPoint) const {
            Point<double> temp(unitPoint);
            temp[0] *= scale;
            temp[1] *= scale;
            temp[2] *= scale;

            temp[0] += extent.lo[0];
            temp[1] += extent.lo[1];
            temp[2] += extent.lo[2];

            return temp;
        }

    private:
        Extent<double> extent;
        double scale;
        double oneOverScale;
    };
}

#endif
