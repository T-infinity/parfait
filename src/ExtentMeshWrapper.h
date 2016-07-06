#pragma once
#include <vector>
#include "Extent.h"

namespace Parfait{
    class ExtentMeshWrapper{
    public:
        ExtentMeshWrapper() = default;
        ExtentMeshWrapper(const Extent<double>& e):extents({e}){}
        ExtentMeshWrapper(const std::vector<Extent<double>>& vec):extents(vec){}
        int numberOfNodes() {return 8*extents.size();}
        int numberOfCells() {return extents.size();}
        int numberOfNodesInCell(int i) {return 8;}
        Point<double> getNode(int i) {
            auto& e = extents[i/8];
            int vertex = i%8;
            auto p = e.lo;
            if(1 == vertex or 2 == vertex or 5 == vertex or 6 == vertex)
                p[0] += e.getLength_X();
            if(2 == vertex or 3 == vertex or 6 == vertex or 7 == vertex)
                p[1] += e.getLength_Y();
            if(3 < vertex)
                p[2] += e.getLength_Z();
            return p;
        }
        std::array<int,8> getNodesInCell(int i){
            int n = i*8;
            return {n,n+1,n+2,n+3,n+4,n+5,n+6,n+7};
        };
        void addExtent(const Extent<double>& e){ extents.push_back(e); }
    private:
        std::vector<Parfait::Extent<double>> extents;
    };
}

