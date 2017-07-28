#pragma once
#include "../MessagePasser/MessagePasser.h"
#include "ExtentBuilder.h"

namespace Parfait {
  namespace ParallelExtent {

    template<typename T>
    Extent<T> getBoundingBox(std::shared_ptr<MessagePasser> mp, Extent<T> e) {
        int root = 0;
        std::vector<Extent<T>> extents;
        mp->Gather(e, extents, root);

        Extent<T> out;
        if (mp->Rank() == root)
            out = ExtentBuilder::getBoundingBox(extents);

        mp->Broadcast(out, root);
        return out;
    }

    template<typename T>
    Extent<T> getBoundingBox(std::shared_ptr<MessagePasser> mp,
                             std::vector<Extent<T>>& boxes) {
        auto out = ExtentBuilder::getBoundingBox(boxes);
        return getBoundingBox(mp, out);
    }
  }
}