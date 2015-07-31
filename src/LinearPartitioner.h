#ifndef PARFAIT_LINEARPARTITIONER_H
#define PARFAIT_LINEARPARTITIONER_H
#include <tuple>

namespace Parfait {
  namespace LinearPartitioner {
    struct Range {
        Range(int start_in, int end_in)
                : start(start_in), end(end_in) { }
        int start;
        int end;
    };

    Range getRangeForWorker(int id, int numWorkItems, int numWorkers);
    Range getRangeForCurrentProc(int numWorkItems);
    Range getRangeForProc(int procId, int numWorkItems);
  }
}

#include "LinearPartitioner.hpp"
#endif 
