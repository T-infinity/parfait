#include "parfait_checkpoint.h"

void checkpoint(std::string s){
    using namespace MessagePasser;
    Barrier();
    if(0 == Rank()){
        //printf("--Checkpoint--\n  File: %s\n  Line: %d\n",
        //        __FILE__,__LINE__);
        printf("-------Checkpoint-----------------------------\n");
        printf("%s\n",s.c_str());
        fflush(stdout);
        printf("----------------------------------------------\n");
    }
}

void checkpoint(){checkpoint("");}
