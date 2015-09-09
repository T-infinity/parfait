#ifndef PARFAIT_MESHBASICPARALLEL_H
#define PARFAIT_MESHBASICPARALLEL_H

#include <memory>
#include "MeshConnectivity.h"
#include "MeshParallelMetadata.h"

namespace Parfait {
    class MeshBasicParallel {
        std::shared_ptr<MeshConnectivity> connectivity;
        std::shared_ptr<MeshParallelMetadata> metaData;
    };
}

#endif
