#include "timing.h"

void printTimeDifference(Clock::time_point t0, Clock::time_point t1){
    milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
    std::cout << ms.count() << "ms\n";
}
