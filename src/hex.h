#ifndef __HEX_H
#define __HEX_H

#include <Point.h>
#include <Extent.h>

namespace Parfait {
    class Hex {
    public:
        Hex() { }

        Hex(Extent<double> extent);
        Point<double> points[8];

        const Point<double> &operator[](const int i) const {
            return points[i];
        }

        Point<double> &operator[](const int i) {
            return points[i];
        }

        void Print() {
            for (unsigned int i = 0; i < 8; i++) {
                points[i].Print();
            }
        }
    };
}



//   Create a hex from an extent box.  Nodes are labeled this way.
//
//     +   7 ---- 6
//    y   /|     /|
//   -   / |    / |
//      4 -3-- 5  2
//  +   | /    | /
//  z   |/     |/
//  -   0 ---- 1
//       - x+


inline Parfait::Hex::Hex(Extent<double> extent){
    points[0] = extent.lo;
    points[6] = extent.hi;

    points[1][0] = extent.hi[0];
    points[1][1] = extent.lo[1];
    points[1][2] = extent.lo[2];

    points[2][0] = extent.hi[0];
    points[2][1] = extent.hi[1];
    points[2][2] = extent.lo[2];

    points[3][0] = extent.lo[0];
    points[3][1] = extent.hi[1];
    points[3][2] = extent.lo[2];

    points[4][0] = extent.lo[0];
    points[4][1] = extent.lo[1];
    points[4][2] = extent.hi[2];

    points[5][0] = extent.hi[0];
    points[5][1] = extent.lo[1];
    points[5][2] = extent.hi[2];

    points[7][0] = extent.lo[0];
    points[7][1] = extent.hi[1];
    points[7][2] = extent.hi[2];
}


#endif
