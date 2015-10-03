#ifndef PARFAIT_LINEARPARTITIONER_H
#define PARFAIT_LINEARPARTITIONER_H
#include <tuple>

namespace Parfait {
  namespace LinearPartitioner {
    template <typename T>
    class Range {
    public:
        Range() = default;
        Range(T start, T end);
        T start;
        T end;
        bool owns(T id) const;
    };
    struct OwnerLocalIdPair{
        OwnerLocalIdPair() = default;
        OwnerLocalIdPair(long owner, long id);
        long owner;
        long id;
    };

    long getWorkerOfWorkItem(long itemId, long numWorkItems, long numWorkers);
    Range<long> getRangeForWorker(long id, long numWorkItems, long numWorkers);
    Range<long> getRangeForCurrentProc(long numWorkItems);
    Range<long> getRangeForProc(long procId, long numWorkItems);
    OwnerLocalIdPair getOwnerLocalIdPairOfGlobalItem(long globalItemId, long numWorkItems, long numWorkers);
    long getLocalIdOfItemOnWorker(long globalItemId, long workerId, long numWorkItems, long numWorkers);

  }
}

#include "LinearPartitioner.hpp"
#endif 
