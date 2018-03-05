
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
#pragma once
#include <string.h>
#include <stdio.h>

#ifdef __APPLE__
#include<mach/mach.h>
#endif

namespace Parfait{

#ifdef __APPLE__
    inline int getMemoryUseage(){
        struct task_basic_info t_info;
        mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
        if (KERN_SUCCESS != task_info(mach_task_self(),
                                      TASK_BASIC_INFO, (task_info_t)&t_info,
                                      &t_info_count))
        {
            return -1;
        }
        return t_info.resident_size/(1024*1024);
    }
#else
  int parseLine(char* line){
    int i = strlen(line);
    while (*line < '0' || *line > '9') line++;
    line[i-3] = '\0';
    i = atoi(line);
    return i;
  }

  inline int getMemoryUseage(){ //Note: this value is in MB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];


    while (fgets(line, 128, file) != NULL){
      if (strncmp(line, "VmRSS:", 6) == 0){
        result = parseLine(line);
        break;
      }
    }
    fclose(file);
    return result / 1024;
  }

#endif
}
