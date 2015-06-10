#include "adt_3d_point.h"
#include <stdexcept>

using namespace Parfait;
Adt3DPoint::Adt3DPoint(const Extent<double> &domain)
    : unitTransformer(domain)
{}

void Adt3DPoint::store(int Id, const Point<double> &point) {
    auto storePoint = unitTransformer.ToUnitSpace(point);
    for(int i = 0; i < 3; i++){
        if(storePoint[i] < 0 or storePoint[i] > 1.0)
            throw std::logic_error("Cannot store points in ADT which are outside the ADT domain.");
    }
    adt.store(Id,storePoint.data());
}

std::vector<int> Adt3DPoint::retrieve(const Extent<double> &domain) const {
    Extent<double> search(unitTransformer.ToUnitSpace(domain.lo),
                  unitTransformer.ToUnitSpace(domain.hi));
    return adt.retrieve(&search.lo[0]);
}

int Adt3DPoint::getNodeId(const Point<double> &p) {
    auto output = retrieve({p, p});
    if (output.size() != 1)
        throw std::logic_error("Adt has unexpected number of points at node Id");
    return output[0];
}
