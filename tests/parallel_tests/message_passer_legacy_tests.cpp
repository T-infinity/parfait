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
		//test scatter for integers
		int root = 0;
		std::vector<int> vec;
		int recv_value;
		if(Rank() == root)
			for(int i=0;i<NumberOfProcesses();i++)
				vec.push_back(i);
		Scatter(vec,recv_value,root);	
		LONGS_EQUAL(Rank(),recv_value);
	}
	
	{
		//test scatter for floats
		int root = 0;
		std::vector<float> vec;
		float recv_value;
		float junk = 1.7e-5;
		if(Rank() == root)
			for(int i=0;i<NumberOfProcesses();i++)
				vec.push_back(junk+(float)i);
		Scatter(vec,recv_value,root);	
		DOUBLES_EQUAL(junk+(float)Rank(),recv_value,MPI_FLOAT_TOL);
	}
	
	{
		//test scatter for doubles
		int root = 0;
		std::vector<double> vec;
		double recv_value;
		double junk = 1.7e-13;
		if(Rank() == root)
			for(int i=0;i<NumberOfProcesses();i++)
				vec.push_back(junk+(double)i);
		Scatter(vec,recv_value,root);	
		DOUBLES_EQUAL(junk+(double)Rank(),recv_value,MPI_DOUBLE_TOL);
	}
	
	{
		//test scatter for multiple integers
		int root = 0;
		std::vector<int> vec;
		std::vector<int> recv_vec;
		if(Rank() == root)
			for(int i=0;i<NumberOfProcesses();i++)
			{
				vec.push_back(i);
				vec.push_back(i+3);
			}
		Scatter(vec,recv_vec,root);	
		LONGS_EQUAL(2,(int)recv_vec.size());	
		LONGS_EQUAL(Rank(),recv_vec[0]);
		LONGS_EQUAL(Rank()+3,recv_vec[1]);
	}
	
	{
		//test scatter for multiple floats
		int root = 0;
		std::vector<float> vec;
		std::vector<float> recv_vec;
		float junk = 1.7e-5;
		if(Rank() == root)
			for(int i=0;i<NumberOfProcesses();i++)
			{
				vec.push_back(junk+(float)i);
				vec.push_back(junk+(float)(i+3));
			}
		Scatter(vec,recv_vec,root);	
		LONGS_EQUAL(2,(int)recv_vec.size());	
		DOUBLES_EQUAL(junk+(float)Rank(),recv_vec[0],MPI_FLOAT_TOL);
		DOUBLES_EQUAL(junk+(float)(Rank()+3),recv_vec[1],MPI_FLOAT_TOL);
	}
	
	{
		//test scatter for multiple doubles
		int root = 0;
		std::vector<double> vec;
		std::vector<double> recv_vec;
		double junk = 1.7e-13;
		if(Rank() == root)
			for(int i=0;i<NumberOfProcesses();i++)
			{
				vec.push_back(junk+(double)i);
				vec.push_back(junk+(double)(i+3));
			}
		Scatter(vec,recv_vec,root);	
		LONGS_EQUAL(2,(int)recv_vec.size());	
		DOUBLES_EQUAL(junk+(double)Rank(),recv_vec[0],MPI_DOUBLE_TOL);
		DOUBLES_EQUAL(junk+(double)(Rank()+3),recv_vec[1],MPI_DOUBLE_TOL);
	}

	{
		// test Scatterv for integers
		int root = 0;
		std::vector<int> vec;
		std::vector<int> recv_vec;
		int length = NumberOfProcesses() + 7;
		if(Rank() == root)
		{
			for(int i=0;i<length;i++)
				vec.push_back(i);
		}
		Scatterv(vec,recv_vec,root);
		// check that each proc has the right size vector after the call
		if(Rank() < length % NumberOfProcesses())
		{
			LONGS_EQUAL(length/NumberOfProcesses()+1,(int)recv_vec.size());
		}
		else
		{
			LONGS_EQUAL(length/NumberOfProcesses(),(int)recv_vec.size());
		}
		// check that each proc has the correct range of values
		int start = 0;
		for(int i=0;i<Rank();i++)
		{
			if(i < length % NumberOfProcesses())
			{
				start += length/NumberOfProcesses() + 1;
			}
			else
			{
				start += length/NumberOfProcesses();
			}
		}
		for(int i=0;i<(int)recv_vec.size();i++)
		{
			LONGS_EQUAL(start+i,recv_vec[i]);
		}
	}
	
	{
		// test Scatterv for floats
		int root = 0;
		std::vector<float> vec;
		std::vector<float> recv_vec;
		float junk = 1.7e-5;
		int length = NumberOfProcesses() + 7;
		if(Rank() == root)
		{
			for(int i=0;i<length;i++)
				vec.push_back(junk+(float)i);
		}
		Scatterv(vec,recv_vec,root);
		// check that each proc has the correct range of values
		int start = 0;
		for(int i=0;i<Rank();i++)
		{
			if(i < length % NumberOfProcesses())
			{
				start += length/NumberOfProcesses() + 1;
			}
			else
			{
				start += length/NumberOfProcesses();
			}
		}
		for(int i=0;i<(int)recv_vec.size();i++)
		{
			DOUBLES_EQUAL(junk+(float)(start+i),recv_vec[i],MPI_FLOAT_TOL);
		}
	}
	
	{
		// test Scatterv for doubles
		int root = 0;
		std::vector<double> vec;
		std::vector<double> recv_vec;
		double junk = 1.7e-13;
		int length = NumberOfProcesses() + 7;
		if(Rank() == root)
		{
			for(int i=0;i<length;i++)
				vec.push_back(junk+(double)i);
		}
		Scatterv(vec,recv_vec,root);
		// check that each proc has the correct range of values
		int start = 0;
		for(int i=0;i<Rank();i++)
		{
			if(i < length % NumberOfProcesses())
			{
				start += length/NumberOfProcesses() + 1;
			}
			else
			{
				start += length/NumberOfProcesses();
			}
		}
		for(int i=0;i<(int)recv_vec.size();i++)
		{
			DOUBLES_EQUAL(junk+(double)(start+i),recv_vec[i],MPI_FLOAT_TOL);
		}
	}


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



