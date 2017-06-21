#pragma once
#include <vector>
#include <STL.h>
#include <Point.h>
#include <memory>
#include "MessagePasser.h"

class DistanceCalculator {
public:

    enum {TRI_3, QUAD_4};

    template <typename FillPoint, typename FillCell, typename CellType>
    DistanceCalculator(std::shared_ptr<MessagePasser> mp, FillPoint fillPoint,FillCell fillCell, CellType cellType,int cell_count)
            : mp(mp), stl(extractFacets(fillPoint,fillCell,cellType,cell_count)), searchSTL(stl){}



    inline Parfait::Point<double> closest(const Parfait::Point<double>& p) const {
        return searchSTL.getClosestPoint(p);
    }

    double distance(const Parfait::Point<double>& p) const {
        return (closest(p) - p).magnitude();
    }

private:
    std::shared_ptr<MessagePasser> mp;
    const Parfait::STL::STL stl;
    const Parfait::STL::SearchSTL searchSTL;

    template <typename FillPoint, typename FillCell, typename CellType>
    std::vector<Parfait::Facet> extractFacets(FillPoint,FillCell,CellType,int);
};

template <typename FillPoint, typename FillCell>
void appendTriFacet(std::vector<Parfait::Facet> &facets, FillPoint fillPoint, FillCell fillCell,
                                        int cell_id) {

    std::array<int, 3> cell;
    fillCell(cell_id, cell.data());
    Parfait::Facet f;
    int i = 0;
    for(int n : cell){
        fillPoint(n, f[i++].data());
    }

    facets.push_back(f);
}
template <typename FillPoint, typename FillCell>
void appendQuadFacets(std::vector<Parfait::Facet> &facets, FillPoint fillPoint, FillCell fillCell,
                    int cell_id) {
    std::array<int, 4> cell;
    fillCell(cell_id, cell.data());
    Parfait::Facet f1;
    Parfait::Facet f2;

    std::array<Parfait::Point<double>, 4> p;
    int i = 0;
    for (int n : cell) {
        fillPoint(n, p[i++].data());
    }

    facets.push_back({p[0], p[1], p[2]});
    facets.push_back({p[2], p[3], p[0]});

}

template <typename FillPoint, typename FillCell, typename CellType>
std::vector<Parfait::Facet> DistanceCalculator::extractFacets(FillPoint fillPoint, FillCell fillCell, CellType cellType, int cell_count){

    std::vector<Parfait::Facet> send_facets;
    std::vector<Parfait::Facet> facets;
    for(int cell_id = 0; cell_id < cell_count; cell_id++){
        auto type = cellType(cell_id);

        if(type == DistanceCalculator::TRI_3)
            appendTriFacet(send_facets, fillPoint, fillCell, cell_id);
        if(type == DistanceCalculator::QUAD_4)
            appendQuadFacets(send_facets, fillPoint, fillCell, cell_id);
    }
    mp->AllGatherv(send_facets, facets);
    return facets;
};

