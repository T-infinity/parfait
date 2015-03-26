#ifndef MESSAGE_PASSER_STATUS_H
#define MESSAGE_PASSER_STATUS_H
#include <mpi.h>

class MessageStatus{
    public:
        MessageStatus(){}
        MPI_Request* request() {return &r;}
    private:
        MPI_Request r;
};


#endif
