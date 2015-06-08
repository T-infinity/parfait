#include "adt_3d_extent.h"

using namespace Parfait;
Adt3DExtent::Adt3DExtent(const Extent &domain)
        : unitTransformer(domain)
{}

void Adt3DExtent::store(int Id, const Extent &extent)
{
    Extent store;
    store.lo = unitTransformer.ToUnitSpace(extent.lo);
    store.hi = unitTransformer.ToUnitSpace(extent.hi);
    adt.store(Id, &store.lo[0]);
}

std::vector<int> Adt3DExtent::retrieve(const Extent &domain) const
{
    Extent adtDomain = unitTransformer.getDomain();
    if(not adtDomain.contains(domain))
        return {};

    Extent search(unitTransformer.ToUnitSpace(domain.lo),
                  unitTransformer.ToUnitSpace(domain.hi));
    return adt.retrieve(&search.lo[0]);
}