#pragma once
#include <vector>
#include <Point.h>
#include <ParallelMesh.h>
#include <SyncPattern.h>
#include <MessagePasser.h>

namespace Helpers {
  Parfait::Point<double> computeTriangleArea(
          const Parfait::Point<double>& a,
          const Parfait::Point<double>& b,
          const Parfait::Point<double>& c);

  Parfait::Point<double> average(const std::vector<Parfait::Point<double>>& points);

  Parfait::Point<double> computeCellCenter(
          const std::shared_ptr<Parfait::ParallelMesh>& mesh,
          int cell_id);

  template<typename Mesh>
  std::map<long, int> getNodeGlobalToLocal(Mesh& mesh) {
      std::map<long, int> global_to_local;
      for (int n = 0; n < mesh.nodeCount(); n++) {
          global_to_local[mesh.globalNodeId(n)] = n;
      }
      return global_to_local;
  };

  template<typename Mesh>
  std::pair<std::vector<long>, std::vector<long>> buildHaveNeedForNodes(
          std::shared_ptr<MessagePasser> messagePasser,
          Mesh& mesh) {
      std::vector<long> have;
      std::vector<long> need;
      int me = messagePasser->Rank();
      for (int n = 0; n < mesh.nodeCount(); n++) {
          if (mesh.nodeOwner(n) == me)
              have.push_back(mesh.globalNodeId(n));
          else
              need.push_back(mesh.globalNodeId(n));
      }
      return {have, need};
  }

  template<typename Mesh>
  SyncPattern getNodeSyncPattern(std::shared_ptr<MessagePasser> mp, Mesh& mesh) {
      std::vector<long> have;
      std::vector<long> need;
      std::tie(have, need) = buildHaveNeedForNodes(mp, mesh);

      auto s = SyncPattern(buildSyncPattern(mp, have, need));
      return s;
  }
}
