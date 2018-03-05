
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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
inline Parfait::Extent<double> Parfait::Adt3DExtent::boundingExtent() const {
    return unitTransformer.getDomain();
}

