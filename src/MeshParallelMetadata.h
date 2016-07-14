#pragma once

#include <vector>

namespace Parfait {
    class MeshParallelMetaData {
    public:
        std::vector<double> xyz;
        std::vector<int> triangleTags;
        std::vector<int> quadTags;

        std::vector<long> globalNodeIds;
        std::vector<int> nodeOwnershipDegree;
        std::vector<int> nodeComponentIds;

        int getTagForFace(int id) {
            int ntri = triangleTags.size();
            if(id < ntri)
                return triangleTags[id];
            return quadTags[id - ntri];
        }
    };
}
