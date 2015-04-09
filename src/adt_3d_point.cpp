#include "adt_3d_point.h"
#include <stdexcept>

Adt3DPoint::Adt3DPoint(const Extent &domain)
    : unitTransformer(domain)
{}

void Adt3DPoint::store(int Id, const Point &point)
{
    Point storePoint = unitTransformer.ToUnitSpace(point);
    for(int i = 0; i < 3; i++){
        if(storePoint[i] < 0 or storePoint[i] > 1.0)
            throw std::logic_error("Cannot store points in ADT which are outside the ADT domain.");
    }
    adt.store(Id, &storePoint[0]);
}

std::vector<int> Adt3DPoint::retrieve(const Extent &domain) const {
    Extent search(unitTransformer.ToUnitSpace(domain.lo), 
                  unitTransformer.ToUnitSpace(domain.hi));
    return adt.retrieve(&search.lo[0]);
}

int Adt3DPoint::getNodeId(const Point &p) {
    auto output = retrieve({p, p});
    if (output.size() != 1)
        throw std::logic_error("Adt has unexpected number of points at node Id");
    return output[0];
}
