#ifndef PARFAIT_MESHPARALLELMETADATA_H
#define PARFAIT_MESHPARALLELMETADATA_H

#include <vector>

namespace Parfait {
    class MeshParallelMetadata {
        std::vector<double> xyz;
        std::vector<int> triangleTags;
        std::vector<int> quadTags;

        std::vector<long> globalNodeIds;
        std::vector<int> nodeOwnershipDegree;
    };
}

#endif
