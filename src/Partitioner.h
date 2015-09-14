#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>
#include <memory>
#include <MeshBasicParallel.h>

namespace Parfait {
    namespace Partitioner {
        std::vector<int> generatePartVector(const std::vector<std::vector<long>>& node_to_node);
    }
}
#include "Partitioner.hpp"
#endif
