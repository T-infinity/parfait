#pragma once

#include <chrono>
#include <iostream>
namespace Parfait {
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    typedef std::chrono::seconds seconds;
    typedef std::chrono::minutes minutes;
    typedef std::chrono::hours hours;

    inline auto Now() {return Clock::now();}

    inline std::string timeDifferenceInMillisecondsAsString(Clock::time_point t0, Clock::time_point t1);
    inline std::string readableElapsedTimeAsString(Clock::time_point t0, Clock::time_point t1);

}
#include "Timing.hpp"