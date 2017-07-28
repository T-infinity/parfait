#pragma once
#include <tuple>
#include <vector>

namespace Parfait {
  namespace LinearPartitioner {
    template <typename T>
    class Range {
    public:
        Range(){}
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

    long getWorkerOfWorkItem(long item_id, long numWorkItems, long numWorkers);
    Range<long> getRangeForWorker(long worker_id, long numWorkItems, long numWorkers);
    Range<long> getRangeForCurrentProc(long numWorkItems);
    Range<long> getRangeForProc(long procId, long numWorkItems);
    OwnerLocalIdPair getOwnerLocalIdPairOfGlobalItem(long globalItemId, long numWorkItems, long numWorkers);
    long getLocalIdOfItemOnWorker(long globalItemId, long workerId, long numWorkItems, long numWorkers);

    std::vector<long> getOwnersOfEveryWorkItem(long num_work_items, long num_workers);

  }
}

#include "LinearPartitioner.hpp"