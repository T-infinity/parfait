#include <catch.hpp>
#include <MessagePasser.h>

TEST_CASE("Self Send"){
    std::vector<int> send(100,1);
    std::vector<int> recv;
    int tag = 0;

    if (MessagePasser::Rank() == 0) {
        MPI_Request request;
        MPI_Isend(send.data(), send.size(), MPI_INT, 0, tag, MPI_COMM_WORLD, &request);

        MPI_Status status;
        MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_INT, &count);
        if(count != 100)
            throw std::logic_error("Count incorrect size");
        recv.resize(count);
        MPI_Recv(recv.data(), count, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(recv[0] != 1)
            throw std::logic_error("Recv incorrect value");
        MPI_Wait(&request, &status);
    }
}