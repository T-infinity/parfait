#ifndef PARFAIT_LINEARPARTITIONER_HPP
#define PARFAIT_LINEARPARTITIONER_HPP

#include <MessagePasser.h>

namespace Parfait {
  namespace LinearPartitioner {
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
    inline void throwIfIdInvalid(long id, long numWorkers);

    inline void throwIfIdInvalid(long id, long numWorkers) {
        if (id >= numWorkers or id < 0)
            throw std::logic_error("Requested range for invalid worker Id: " + std::to_string(id));
    };
    inline Range getRangeForWorker(long id, long numWorkItems, long numWorkers) {
        throwIfIdInvalid(id, numWorkers);
        int left_overs = numWorkItems % numWorkers;
        int work_per_worker = numWorkItems / numWorkers;
        int start = computeStartGivenLeftOvers(id, work_per_worker, left_overs);
        int end = computeStartGivenLeftOvers(id + 1, work_per_worker, left_overs);
        return {start, end};
    }

    inline Range getRangeForProc(long procId, long numWorkItems){
        return getRangeForWorker(procId, numWorkItems, MessagePasser::NumberOfProcesses());
    }

    inline Range getRangeForCurrentProc(long numWorkItems){
        return getRangeForWorker(MessagePasser::Rank(), numWorkItems, MessagePasser::NumberOfProcesses());
    }
  }
}
#endif 
