#include <stdexcept>

inline Parfait::Adt3DPoint::Adt3DPoint(const Parfait::Extent<double> &domain)
    : unitTransformer(domain)
{}

inline void Parfait::Adt3DPoint::store(int Id, const Parfait::Point<double> &point) {
    auto storePoint = unitTransformer.ToUnitSpace(point);
    for(int i = 0; i < 3; i++){
        if(storePoint[i] < 0 or storePoint[i] > 1.0) {
            throw std::logic_error("Cannot store points in ADT which are outside the ADT rootDomain.");
        }
    }
    adt.store(Id,storePoint.data());
}

inline std::vector<int> Parfait::Adt3DPoint::retrieve(const Parfait::Extent<double> &domain) const {
    Parfait::Extent<double> search(unitTransformer.ToUnitSpace(domain.lo),
                  unitTransformer.ToUnitSpace(domain.hi));
    return adt.retrieve(&search.lo[0]);
}

inline int Parfait::Adt3DPoint::getNodeId(const Parfait::Point<double> &p) {
    auto output = retrieve({p, p});
    if (output.size() != 1)
        throw std::logic_error("Adt has unexpected number of points at node Id");
    return output[0];
}
