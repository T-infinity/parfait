#include "MessagePasser.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("MessagePasser basic stuff"){

	REQUIRE(MPI_INT == Type(int()));
	REQUIRE(MPI_FLOAT == Type(float()));
	REQUIRE(MPI_DOUBLE == Type(double()));
	REQUIRE(MPI_LONG == Type(long()));

	int myRank=0;
	MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
	REQUIRE(myRank == Rank());

	int size=0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	REQUIRE(size == NumberOfProcesses());

	// not sure how to test barrier...but it works...promise
	Barrier();

}



