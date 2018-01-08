#pragma once
#include <vector>
#include "Point.h"
#include "ParallelMesh.h"
#include "SyncPattern.h"
#include <MessagePasser/MessagePasser.h>
#include "SyncPattern.h"

namespace Parfait {
  namespace Metrics {

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

    inline Parfait::Point<double> computeTriangleArea(
        const Parfait::Point<double> &a,
        const Parfait::Point<double> &b,
        const Parfait::Point<double> &c) {
        auto v1 = b - a;
        auto v2 = c - a;
        return 0.5 * Point<double>::cross(v1, v2);
    }

    inline Parfait::Point<double> average(const std::vector <Parfait::Point<double>> &points) {
        Parfait::Point<double> avg = {0, 0, 0};
        for (auto &p : points)
            avg += p;
        return avg / (double) points.size();
    }

    template<typename Mesh>
    std::map<long, int> getNodeGlobalToLocal(Mesh &mesh) {
        std::map<long, int> global_to_local;
        for (int n = 0; n < mesh.nodeCount(); n++) {
            global_to_local[mesh.globalNodeId(n)] = n;
        }
        return global_to_local;
    };

    template<typename Mesh>
    std::pair <std::vector<long>, std::vector<long>> buildHaveNeedForNodes(
        MessagePasser messagePasser,
        Mesh &mesh) {
        std::vector<long> have;
        std::vector<long> need;
        int me = messagePasser.Rank();
        for (int n = 0; n < mesh.nodeCount(); n++) {
            if (mesh.nodeOwner(n) == me)
                have.push_back(mesh.globalNodeId(n));
            else
                need.push_back(mesh.globalNodeId(n));
        }
        return {have, need};
    }

    template<typename Mesh>
    Parfait::SyncPattern getNodeSyncPattern(MessagePasser mp, Mesh &mesh) {
        std::vector<long> have;
        std::vector<long> need;
        std::tie(have, need) = buildHaveNeedForNodes(mp, mesh);

        auto s = Parfait::SyncPattern(Parfait::SyncPattern::build(mp, have, need));
        return s;
    }
  }
}
