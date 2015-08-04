#ifndef PARFAIT_LINEARPARTITIONER_HPP
#define PARFAIT_LINEARPARTITIONER_HPP

#include <MessagePasser.h>

namespace Parfait {
  namespace LinearPartitioner {
    int computeStartGivenLeftOvers(int id, int work_per_worker, int left_overs) {
        int start;
        if (id <= left_overs)
            start = id * (work_per_worker + 1);
        else {
            int add = left_overs * (work_per_worker + 1);
            start = (id - left_overs) * (work_per_worker) + add;
        }
        return start;
    }
    void throwIfIdInvalid(int id, int numWorkers);

    void throwIfIdInvalid(int id, int numWorkers) {
        if (id >= numWorkers or id < 0)
            throw std::logic_error("Requested range for invalid worker Id: " + std::to_string(id));
    };
    Range getRangeForWorker(int id, int numWorkItems, int numWorkers) {
        throwIfIdInvalid(id, numWorkers);
        int left_overs = numWorkItems % numWorkers;
        int work_per_worker = numWorkItems / numWorkers;
        int start = computeStartGivenLeftOvers(id, work_per_worker, left_overs);
        int end = computeStartGivenLeftOvers(id + 1, work_per_worker, left_overs);
        return {start, end};
    }

    Range getRangeForProc(int procId, int numWorkItems){
        return getRangeForWorker(procId, numWorkItems, MessagePasser::NumberOfProcesses());
    }

    Range getRangeForCurrentProc(int numWorkItems){
        return getRangeForWorker(MessagePasser::Rank(), numWorkItems, MessagePasser::NumberOfProcesses());
    }
  }
}
#endif 
