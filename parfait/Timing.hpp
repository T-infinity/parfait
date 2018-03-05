
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include <sstream>
namespace Parfait{
    inline std::string timeDifferenceInMillisecondsAsString(Clock::time_point t0, Clock::time_point t1){
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
        std::string out = std::to_string(ms.count()) + "ms";
        return out;
    }

    inline std::string readableElapsedTimeAsString(Clock::time_point t0, Clock::time_point t1){
        hours h = std::chrono::duration_cast<hours>(t1-t0);
        minutes m = std::chrono::duration_cast<minutes>(t1-t0);
        seconds s = std::chrono::duration_cast<seconds>(t1-t0);
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1-t0);

        long number_of_hours = h.count();
        long number_of_minutes = m.count() - 60*number_of_hours;
        long number_of_seconds = s.count() - 360*number_of_hours - 60*number_of_minutes;
        long number_of_milliseconds = ms.count() - 3600000*number_of_hours
                                                 - 60000*number_of_minutes
                                                 - 1000*number_of_seconds;

        std::string out = "";
        if(0 < number_of_hours)
            out.append(std::to_string(number_of_hours) + " hours ");
        if(0 < number_of_minutes)
            out.append(std::to_string(number_of_minutes) + " minutes ");
        out.append(std::to_string(number_of_seconds) + "." + std::to_string(number_of_milliseconds) + " seconds");
        return out;
    }
}
