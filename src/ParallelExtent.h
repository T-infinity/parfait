#ifndef PARFAIT_PARALLELEXTENT_H
#define PARFAIT_PARALLELEXTENT_H

#include <Extent.h>

namespace Parfait {
  namespace ParallelExtent {
    template<typename T>
    Extent<T> getBoundingBox(Extent<T> e);
    template<typename T>
    Extent<T> getBoundingBox(std::vector<Extent<T>> &boxes);

  }
}

#include "ParallelExtent.hpp"
#endif 
