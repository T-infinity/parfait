#pragma once

#include "Adt.h"
#include "Point.h"
#include "UnitTransformer.h"

namespace Parfait {
    class Adt3DExtent {
    public:
        Adt3DExtent() = delete;
        Adt3DExtent(const Extent<double> &domain);
        void store(int id, const Extent<double> &extent);
        std::vector<int> retrieve(const Extent<double> &domain) const;
        void removeFirst(int id, const Extent<double>& e);
        Parfait::Extent<double> boundingExtent() const;
    private:
        UnitTransformer unitTransformer;
        Adt<6> adt;
    };
}
#include "Adt3dExtent.hpp"
