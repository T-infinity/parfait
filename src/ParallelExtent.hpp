#ifndef PARFAIT_PARALLELEXTENT_HPP
#define PARFAIT_PARALLELEXTENT_HPP

#include <ExtentBuilder.h>

namespace Parfait{
  namespace ParallelExtent{

    template <typename T>
    Extent<T> getBoundingBox(Extent <T> e){
        int root = 0;
        std::vector<Extent<T>> extents;
        MessagePasser::Gather(e, extents, root);

        Extent<T> out;
        if(MessagePasser::Rank() == root)
            out = ExtentBuilder::getBoundingBox(extents);

        MessagePasser::Broadcast(out, root);
        return out;
    }

    template<typename T>
    Extent<T> getBoundingBox(std::vector<Extent<T>> &boxes){
        auto out = ExtentBuilder::getBoundingBox(boxes);
        return getBoundingBox(out);
    }
  }
}
#endif 
