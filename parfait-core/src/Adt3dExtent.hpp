inline Parfait::Adt3DExtent::Adt3DExtent(const Parfait::Extent<double> &domain)
        : unitTransformer(domain)
{}

inline void Parfait::Adt3DExtent::store(int id, const Parfait::Extent<double> &extent) {
    Parfait::Extent<double> store;
    store.lo = unitTransformer.ToUnitSpace(extent.lo);
    store.hi = unitTransformer.ToUnitSpace(extent.hi);
    adt.store(id, &store.lo[0]);
}

inline std::vector<int> Parfait::Adt3DExtent::retrieve(const Parfait::Extent<double> &domain) const {
    auto adtDomain = unitTransformer.getDomain();
    if(not adtDomain.contains(domain))
        return {};

    Parfait::Extent<double> search(unitTransformer.ToUnitSpace(domain.lo),
                  unitTransformer.ToUnitSpace(domain.hi));
    return adt.retrieve(&search.lo[0]);
}
inline void Parfait::Adt3DExtent::removeFirst(int id, const Parfait::Extent<double>& e) {
    auto adtDomain = unitTransformer.getDomain();
    if(not adtDomain.contains(e))
        return;
    Parfait::Extent<double> search(unitTransformer.ToUnitSpace(e.lo),
                                   unitTransformer.ToUnitSpace(e.hi));
    adt.removeFirst(id, search.data());
}

