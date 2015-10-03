#ifndef PARFAIT_LINEARPARTITIONER_HPP
#define PARFAIT_LINEARPARTITIONER_HPP

#include <MessagePasser.h>

namespace Parfait {
  namespace LinearPartitioner {

    inline Range::Range(long start_in, long end_in)
            : start(start_in), end(end_in) { }
    inline OwnerLocalIdPair::OwnerLocalIdPair(long owner_in, long id_in)
            : owner(owner_in), id(id_in) { }

    inline long computeStartGivenLeftOvers(long id, long work_per_worker, long left_overs) {
        long start;
        if (id <= left_overs)
            start = id * (work_per_worker + 1);
        else {
            long add = left_overs * (work_per_worker + 1);
            start = (id - left_overs) * (work_per_worker) + add;
        }
        return start;
    }
    inline void throwIfIdInvalid(long id, long numIds) {
        if (id >= numIds or id < 0)
            throw std::logic_error("Requested partitioning information of invalid Id: " + std::to_string(id));
    };
    inline Range getRangeForWorker(long id, long numWorkItems, long numWorkers) {
        throwIfIdInvalid(id, numWorkers);
        long left_overs = numWorkItems % numWorkers;
        long work_per_worker = numWorkItems / numWorkers;
        long start = computeStartGivenLeftOvers(id, work_per_worker, left_overs);
        long end = computeStartGivenLeftOvers(id + 1, work_per_worker, left_overs);
        return {start, end};
    }

    inline long getWorkerOfWorkItem(long itemId, long numWorkItems, long numWorkers){
        throwIfIdInvalid(itemId, numWorkItems);
        long num_left_workers = numWorkItems % numWorkers;
        long work_items_per_worker = numWorkItems / numWorkers;
        long num_work_items_left = (work_items_per_worker+1) * num_left_workers;
        if(itemId < num_work_items_left)
            return itemId / (work_items_per_worker+1);
        itemId -= num_work_items_left;
            return itemId / work_items_per_worker + num_left_workers;
    }

    inline OwnerLocalIdPair getOwnerLocalIdPairOfGlobalItem(long globalItemId, long numWorkItems, long numWorkers){
        auto owner = getWorkerOfWorkItem(globalItemId, numWorkItems, numWorkers);
        return {owner, getLocalIdOfItemOnWorker(globalItemId, owner, numWorkItems, numWorkers)};
    }

    inline long getLocalIdOfItemOnWorker(long globalItemId, long workerId, long numWorkItems, long numWorkers){
        auto range = getRangeForWorker(workerId, numWorkItems, numWorkers);
        return globalItemId - range.start;
    }

    inline Range getRangeForProc(long procId, long numWorkItems){
        return getRangeForWorker(procId, numWorkItems, MessagePasser::NumberOfProcesses());
    }

    inline Range getRangeForCurrentProc(long numWorkItems){
        return getRangeForWorker(MessagePasser::Rank(), numWorkItems, MessagePasser::NumberOfProcesses());
    }

    inline bool Range::owns(long id) const {
        return id >= start and id < end;
    }
  }
}
#endif 
