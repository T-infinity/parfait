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
