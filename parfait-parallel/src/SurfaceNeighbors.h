#pragma once
#include <vector>
#include <memory>
#include <ParallelMesh.h>

namespace SurfaceNeighbors {
  std::vector<int> buildSurfaceNeighbors(const std::shared_ptr<Parfait::ParallelMesh>& mesh);
}
