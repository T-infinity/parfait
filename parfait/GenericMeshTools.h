#pragma once

#include "Point.h"
#include "VectorTools.h"

#include <cmath>
#include <stdexcept>
#include <set>
#include <array>
namespace Parfait {
  namespace GenericMeshTools {

    template<class MeshType>
    std::vector<std::array<int, 2>> getUniqueEdges(MeshType &mesh_in) {

        int a,b;
        std::set<std::pair<int, int>> edgeSet;
        for(int i=0;i<mesh_in.numberOfCells();++i){
            auto cell = mesh_in.getNodesInCell(i);
            switch(cell.size()){
                case 4:
                    a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[0];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[0];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[1];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[1];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    break;
                case 5:
                    a = cell[4];
                    b = cell[0];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[4];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[4];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[4];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[0];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    break;
                case 6:
                    a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[1];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[0];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[3];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[4];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[5];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[0];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[1];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    break;
                case 8:
                    a = cell[0];
                    b = cell[1];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[1];
                    b = cell[2];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[3];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[3];
                    b = cell[0];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[4];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[5];
                    b = cell[6];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[6];
                    b = cell[7];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[7];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[0];
                    b = cell[4];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[1];
                    b = cell[5];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[2];
                    b = cell[6];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    a = cell[3];
                    b = cell[7];
                    edgeSet.insert(std::make_pair(std::min(a,b),std::max(a,b)));
                    break;
                default:
                    throw std::logic_error("Invalid cell");
            }
        }

        std::vector<std::array<int, 2>> edges;
        edges.reserve(edgeSet.size());
        for (auto edge : edgeSet) {
            std::array<int, 2> e = {edge.first, edge.second};
            edges.push_back(e);
        }

        return edges;
    }
  }
}
