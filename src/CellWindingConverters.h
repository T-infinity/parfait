//
// Created by cdruyorj on 9/1/15.
//

#ifndef PARFAIT_CELL_WINDING_CONVERTERS_H
#define PARFAIT_CELL_WINDING_CONVERTERS_H

#include <functional>

namespace Parfait{
    class CellWindingConverter{
    public:
        CellWindingConverter() = default;
        void convertTet(int* tet) {};
        void convertPyramid(int* pyramid) {}
        void convertPrism(int* prism) {}
        void convertHex(int* hex) {}
    };

    class AflrToCGNS : public CellWindingConverter{
        public:
            void convertPyramid(int *pyramid) {
                int tmp = pyramid[2];
                pyramid[2] = pyramid[3];
                pyramid[3] = pyramid[4];
                pyramid[4] = tmp;
            }
    };

    class CGNSToAflr : public CellWindingConverter{
    public:
        void convertPyramid(int* pyramid){
            int tmp = pyramid[2];
            pyramid[2] = pyramid[4];
            pyramid[4] = pyramid[3];
            pyramid[3] = tmp;
        }
    };

    class CGNSToVtk : public CellWindingConverter{
    public:
        void convertPrism(int* prism){
            std::swap(prism[0],prism[3]);
            std::swap(prism[1],prism[4]);
            std::swap(prism[2],prism[5]);
        }
    };

    class VtkToCGNS : public CellWindingConverter{
    public:
        void convertPrism(int* prism){
            std::swap(prism[0],prism[3]);
            std::swap(prism[1],prism[4]);
            std::swap(prism[2],prism[5]);
        }
    };
}

#endif //PARFAIT_AFLRTOCGNS_H_H
