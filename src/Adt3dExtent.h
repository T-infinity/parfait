#ifndef ADT_3D_EXTENT_H
#define ADT_3D_EXTENT_H

#ifndef ADT_H
#include "Adt.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

#ifndef UNIT_TRANSFORMER_H
#include "unit_transformer.h"
#endif

namespace Parfait {
    class Adt3DExtent {
    public:
        Adt3DExtent() = delete;
        Adt3DExtent(const Extent<double> &domain);
        void store(int Id, const Extent<double> &extent);
        std::vector<int> retrieve(const Extent<double> &domain) const;
    private:
        UnitTransformer unitTransformer;
        Adt<6> adt;
    };
}
#include "Adt3dExtent.hpp"

#endif
