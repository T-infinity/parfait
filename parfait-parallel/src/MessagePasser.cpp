#include "mpi.h"
#include "MessagePasser.h"

MessagePasser::MessagePasser()
    :communicator(MPI_COMM_WORLD) {
}
MessagePasser::MessagePasser(MPI_Comm comm)
: communicator(comm) {

}
MPI_Comm MessagePasser::getCommunicator() const {
    return communicator;
}

void MessagePasser::setCommunicator(MPI_Comm comm) {
    communicator = comm;
}
