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


  template <typename Mesh>
  std::map<long, int> getNodeGlobalToLocal(Mesh mesh){
      std::map<long, int> global_to_local;
      for(int n = 0; n < mesh->numberOfNodes(); n++){
          global_to_local[mesh->getGlobalNodeId(n)] = n;
      }
      return global_to_local;
  };

  template <typename Mesh>
  SyncPattern getNodeSyncPattern(Mesh mesh){
      std::vector<long> have;
      std::vector<long> need;
      for(int n = 0; n < mesh->numberOfNodes(); n++){
          if(not mesh->getOwningRankOfNode(n) == MessagePasser::Rank())
              have.push_back(mesh->getGlobalNodeId(n));
          else
              need.push_back(mesh->getGlobalNodeId(n));
      }
      return SyncPattern(buildSyncPattern(have, need));
  }
}
