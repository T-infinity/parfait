#include "adt_3d_point.h"

Adt3DPoint::Adt3DPoint(const Extent &domain)
    : unitTransformer(domain), adt(ADT_3D_POINT) 
{}

void Adt3DPoint::store(int Id, const Point &point)
{
    Point storePoint = unitTransformer.ToUnitSpace(point);
    adt.store(Id, &storePoint[0]);
}

std::vector<int> Adt3DPoint::retrieve(const Extent &domain) const {
    Extent search(unitTransformer.ToUnitSpace(domain.lo), 
                  unitTransformer.ToUnitSpace(domain.hi));
    return adt.retrieve(&search.lo[0]);
}
