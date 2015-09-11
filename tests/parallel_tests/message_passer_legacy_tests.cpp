#include "MessagePasser.h"
#include "mpi_compare_tols.h"
#include <mpi.h>
#include <vector>
#include <checkpoint.h>

#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(MessagePasserTests)
{
	void setup()
	{
		std::cout << "Running Message Passer Tests" << std::endl;
	}
	void teardown()
	{
	}
};

TEST(MessagePasserTests,Exists)
{
	using namespace MessagePasser;
	using std::vector;
	// check types
	int a=0;
	CHECK_EQUAL(MPI_INT,Type(a));	
	float b=0.0;
	CHECK_EQUAL(MPI_FLOAT,Type(b));
	double c=0.0;
	CHECK_EQUAL(MPI_DOUBLE,Type(c));
	// check rank
	int myRank=0;
	MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
	LONGS_EQUAL(myRank,Rank());
	// check nproc
	int size=0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	LONGS_EQUAL(size,NumberOfProcesses());
	
	Barrier();

}



