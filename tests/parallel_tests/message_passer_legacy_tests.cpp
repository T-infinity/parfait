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

	{
		// test sum for integers
		int root = 0;
		int value = Rank();
		int psum = 0;
		psum = ParallelSum(value,root);
		if(Rank() == root)
		{
			int sum = 0;
			for(int i=0;i<NumberOfProcesses();i++)
				sum += i;
			LONGS_EQUAL(sum,psum);
		}
	}
	
	{
		// test sum for floats
		int root = 0;
		float junk = 1.7e-1;
		float value = junk + (float)Rank();
		float psum = 0.0;
		psum = ParallelSum(value,root);
		if(Rank() == root)
		{
			float sum = 0.0;
			for(int i=0;i<NumberOfProcesses();i++)
				sum += junk + (float)i;
			DOUBLES_EQUAL(sum,psum,MPI_FLOAT_TOL);
		}
	}
	
	{
		// test sum for doubles
		int root = 0;
		double junk = 1.7e-9;
		double value = junk + (double)Rank();
		double psum = 0.0;
		psum = ParallelSum(value,root);
		if(Rank() == root)
		{
			double sum = 0.0;
			for(int i=0;i<NumberOfProcesses();i++)
				sum += junk + (double)i;
			DOUBLES_EQUAL(sum,psum,MPI_DOUBLE_TOL);
		}
	}

	{
		// test max for integer
		int val = Rank();
		int max = ParallelMax(val,0);
		if(Rank() == 0)
			LONGS_EQUAL(NumberOfProcesses()-1,max);
	}
	
	{
		// test max for vector of integers
		int root = 0;
		std::vector<int> vec(NumberOfProcesses(),Rank());
		std::vector<int> result = ParallelMax(vec,root);
		if(Rank() == root)
		{
			for(int i=0;i<NumberOfProcesses();i++)
				LONGS_EQUAL(NumberOfProcesses()-1,result[i]);
		}
	}
	
	{
		// test min for vector of integers
		int root = 0;
		std::vector<int> vec(NumberOfProcesses(),Rank()+7);
		std::vector<int> result = ParallelMin(vec,root);
		if(Rank() == root)
		{
			for(int i=0;i<NumberOfProcesses();i++)
				LONGS_EQUAL(7,result[i]);
		}
	}

	{
		// test parallel sum
		auto sum = MessagePasser::ParallelSum(1);
		LONGS_EQUAL(MessagePasser::NumberOfProcesses(), sum);
	}
}



