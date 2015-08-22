#ifndef PARFAIT_LINEARPARTITIONER_H
#define PARFAIT_LINEARPARTITIONER_H
#include <tuple>

namespace Parfait {
  namespace LinearPartitioner {
    struct Range {
        Range() = default;
        Range(long start_in, long end_in)
                : start(start_in), end(end_in) { }
        long start;
        long end;
    };

    long getWorkerOfWorkItem(long itemId, long numWorkItems, long numWorkers);
    Range getRangeForWorker(long id, long numWorkItems, long numWorkers);
    Range getRangeForCurrentProc(long numWorkItems);
    Range getRangeForProc(long procId, long numWorkItems);
  }
}

#include "LinearPartitioner.hpp"
#endif 
