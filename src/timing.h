#ifndef TIMING_H
#define TIMING_H

#include <chrono>
#include <iostream>
namespace Parfait {
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds milliseconds;

  void printTimeDifference(Clock::time_point t0, Clock::time_point t1);
}
#include "timing.hpp"
#endif
