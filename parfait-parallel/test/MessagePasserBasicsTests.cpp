#include "MessagePasser.h"
#include "catch.hpp"

TEST_CASE("MessagePasser basic stuff"){
    auto mp = std::make_shared<MessagePasser>();

    REQUIRE(MPI_INT == mp->Type(int()));
    REQUIRE(MPI_FLOAT == mp->Type(float()));
    REQUIRE(MPI_DOUBLE == mp->Type(double()));
    REQUIRE(MPI_LONG == mp->Type(long()));

    int myRank=0;
    MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
    REQUIRE(myRank == mp->Rank());

    int size=0;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    REQUIRE(size == mp->NumberOfProcesses());

    // not sure how to test barrier...but it works...promise
    mp->Barrier();
}



