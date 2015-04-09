#ifndef ADT_3D_EXTENT_H
#define ADT_3D_EXTENT_H

#ifndef ADT_H
#include "adt.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

#ifndef UNIT_TRANSFORMER_H
#include "unit_transformer.h"
#endif


class Adt3DExtent
{
    public:
        Adt3DExtent(const Extent &domain);
        void store(int Id, const Extent &extent);
        std::vector<int> retrieve(const Extent &domain) const;
    private:
        UnitTransformer unitTransformer;
        Adt<6> adt;

};

#endif
