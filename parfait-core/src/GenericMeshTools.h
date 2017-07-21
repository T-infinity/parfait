#pragma once

#include "RangeLoop.h"
#include "Point.h"
#include "GenericMesh.h"
#include "VectorTools.h"

#include <cmath>
#include <stdexcept>
#include <set>
#include <array>
namespace Parfait {
  namespace GenericMeshTools {

/*  Computes the volume of a tet given the xyz coords of its
  vertices.  It returns the signed volume, so in order for it
  to be positive, the nodes have to be in the expected order
  (matching ugrid format).

          D
         /|\
        / | \
       /  |  \
      /   |   \
     /    |    \
  A /     |     \ C
    \` ` `|` ` `/
     \    |    /
      \   |   /
       \  |  /
        \ | /
         \|/
          B

*/
    inline double computeTetVolume(const Point<double> &a,
                                   const Point<double> &b,
                                   const Point<double> &c,
                                   const Point<double> &d) {

        auto v1 = a - d;
        auto v2 = b - d;
        auto v3 = c - d;
        auto v = Point<double>::cross(v2, v3);
        return -Point<double>::dot(v1, v) / 6.0;
    }

    template<class MeshType, class container>
    Point<double> computeCenter(MeshType &mesh, const container &nodes) {
        Point<double> center(0, 0, 0);
        for (auto node : nodes) {
            Point<double> a;
            mesh.getNode(node, &a[0]);
            center += a;
        }

        center /= (double) nodes.size();
        return center;
    }

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

    template <typename T>
    Point<T> computeTriangleArea(const Point<T>& a, const Point<T>& b, const Point<T>& c){
        auto v1 = b - a;
        auto v2 = c - a;

        auto area = 0.5 * Point<double>::cross(v1, v2);
        return area;
    }

    template<typename MeshType, class CellType>
    bool containsPoint(MeshType &mesh, CellType &&cell, const Point<double> &p) {
        Point<double> a, b, searchPoint(p);
        for (auto face:cell) {
            // Make tets with the search point and the
            // triangles of each face tesselation and
            // check if the volume is positive for all (p is inside cell)
            auto faceNodeIds = face.getNodes();
            auto faceCentroid = faceCenter(mesh, face);
            int n = faceNodeIds.size();
            for (int i = 0; i < n; i++) {
                mesh.getNode(faceNodeIds[i], a.data());
                mesh.getNode(faceNodeIds[(i + 1) % n], b.data());
                if (0 > CompTetVolume(b, a, faceCentroid, searchPoint))
                    return false;
            }
        }
        return true;
    }

    template<typename MeshType, class CellType>
    double interpolate(MeshType &mesh, CellType &&cell,
                       std::vector<double> &nodeData, double p[3]) {
        double interpolatedValue = 0.0;
        // for now, just average the values at the cell vertices
        // later, this will be tri-linear interpolation, etc.
        for (int nodeId : cell.getNodes()) interpolatedValue += nodeData[nodeId];
        interpolatedValue /= (double) cell.numberOfNodes();
        return interpolatedValue;
    }


    template<typename MeshType, class CellType>
    std::vector<double> calculateInterpolationWeights(MeshType &mesh, CellType &&cell,
                                                      double p[3]) {
        // for now, just return all weights equal
        double w = 1.0 / (double) cell.numberOfNodes();
        std::vector<double> weights(cell.numberOfNodes(), w);

        return weights;
    }
  }
}
