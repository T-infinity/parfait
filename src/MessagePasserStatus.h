#ifndef MESSAGE_PASSER_STATUS_H
#define MESSAGE_PASSER_STATUS_H
#include <mpi.h>
namespace MessagePasser {
    class MessageStatus {
    public:
        MessageStatus() { }
        MPI_Request *request() { return &r; }
    private:
        MPI_Request r;
    };
}

#endif
