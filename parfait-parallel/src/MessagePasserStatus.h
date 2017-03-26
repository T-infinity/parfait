#pragma once
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