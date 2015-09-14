namespace Parfait{
    inline void printTimeDifference(Clock::time_point t0, Clock::time_point t1){
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
        std::cout << ms.count() << "ms\n";
    }

    inline void printElapsedSeconds(Clock::time_point t0, Clock::time_point t1){
        seconds s = std::chrono::duration_cast<seconds>(t1-t0);
        std::cout << s.count() << "seconds \n";
    }

    inline void printReadableElapsedTime(Clock::time_point t0, Clock::time_point t1){
        hours h = std::chrono::duration_cast<hours>(t1-t0);
        minutes m = std::chrono::duration_cast<minutes>(t1-t0);
        seconds s = std::chrono::duration_cast<seconds>(t1-t0);

        long number_of_hours = h.count();
        long number_of_minutes = m.count() - 60*number_of_hours;
        long number_of_seconds = s.count() - 360*number_of_hours - 60*number_of_minutes;

        if(0 < number_of_hours)
            std::cout << number_of_hours << " hours ";
        if(0 < number_of_minutes)
            std::cout << number_of_minutes << " minutes ";
        if(0 < number_of_seconds)
            std::cout << number_of_seconds << " seconds";
        std::cout << std::endl;
    }
}
