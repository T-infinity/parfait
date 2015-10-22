#pragma once
#include <memory>
#include "ParallelMesh.h"
#include "SendReceivePair.h"
#include "SendReceivePairNodeCreator.h"

namespace Parfait {
  namespace SendReceivePairCreator {
    SendReceivePair createNodeBasedPair(std::shared_ptr<ParallelMesh> mesh) {
      NodePairCreator creator(mesh);
      return creator.create();
    }
  };
}

