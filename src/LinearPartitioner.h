#ifndef PARFAIT_LINEARPARTITIONER_H
#define PARFAIT_LINEARPARTITIONER_H
#include <tuple>

namespace Parfait {
  namespace LinearPartitioner {
    struct Range {
        Range() = default;
        Range(long start, long end);
        long start;
        long end;
    };
    struct OwnerLocalIdPair{
        OwnerLocalIdPair() = default;
        OwnerLocalIdPair(long owner, long id);
        long owner;
        long id;
    };

    long getWorkerOfWorkItem(long itemId, long numWorkItems, long numWorkers);
    Range getRangeForWorker(long id, long numWorkItems, long numWorkers);
    Range getRangeForCurrentProc(long numWorkItems);
    Range getRangeForProc(long procId, long numWorkItems);
    OwnerLocalIdPair getOwnerLocalIdPairOfGlobalItem(long globalItemId, long numWorkItems, long numWorkers);
    long getLocalIdOfItemOnWorker(long globalItemId, long workerId, long numWorkItems, long numWorkers);
    }
}

#include "LinearPartitioner.hpp"
#endif 
