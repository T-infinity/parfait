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

	if(Rank() == 0) {
		int value = 0,sum=0,check_sum=0;
		for(int i=1;i<NumberOfProcesses();i++) {
			Recv(value,i);
			sum += value;
			check_sum += i;
		}
		LONGS_EQUAL(check_sum,sum);
	}
	else {
		Send(Rank(),0);
	}

	// test Send/Recv for floats
	if(Rank() == 0) {
		float value = 0,sum=0,check_sum=0;
		for(int i=1;i<NumberOfProcesses();i++)
		{
			Recv(value,i);
			sum += value;
			check_sum += i;
		}
		LONGS_EQUAL(check_sum,sum);
	}
	else {
		Send((float)Rank(),0);
	}

	// test Send/Recv for doubles
	if(Rank() == 0) {
		double value = 0,sum=0,check_sum=0;
		for(int i=1;i<NumberOfProcesses();i++)
		{
			Recv(value,i);
			sum += value;
			check_sum += i;
		}
		LONGS_EQUAL(check_sum,sum);
	}
	else {
		Send((double)Rank(),0);
	}

	// test Recv from any source for integers
	if(Rank() == 0) {
		int value = 0,sum=0,check_sum=0;
		for(int i=1;i<NumberOfProcesses();i++)
		{
			Recv(value);
			sum += value;
			check_sum += i;
		}
		LONGS_EQUAL(check_sum,sum);
	}
	else {
		Send(Rank(),0);
	}

	// test Send for vector of integers
	{
		std::vector<int> vec;
		if(Rank() == 0)
		{
			for(int i=0;i<9;i++)
				vec.push_back(i);
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,9,i);
			}
		}
		else
		{
			Recv(vec,9,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(9,(int)vec.size());
		for(int i=0;i<(int)vec.size();i++)
		{
			LONGS_EQUAL(i,vec[i]);
		}
	}

	// test Send for vector of floats
	{
		std::vector<float> vec;
		float junk = 1.7e-5;
		if(Rank() == 0)
		{
			for(int i=0;i<9;i++)
				vec.push_back(junk+(float)i);
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,9,i);
			}
		}
		else
		{
			Recv(vec,9,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(9,(int)vec.size());
		for(int i=0;i<(int)vec.size();i++)
		{
			DOUBLES_EQUAL(junk+(float)i,vec[i],MPI_FLOAT_TOL);
		}
	}
	
	// test Send for vector of doubles
	{
		std::vector<double> vec;
		double junk = 1.7e-13;
		if(Rank() == 0)
		{
			for(int i=0;i<9;i++)
				vec.push_back(junk+(double)i);
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,9,i);
			}
		}
		else
		{
			Recv(vec,9,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(9,(int)vec.size());
		for(int i=0;i<(int)vec.size();i++)
		{
			DOUBLES_EQUAL(junk+(double)i,vec[i],MPI_FLOAT_TOL);
		}
	}

	// test Send for vector of integers (size not known by receiver)
	{
		std::vector<int> vec;
		if(Rank() == 0)
		{
			for(int i=0;i<9;i++)
				vec.push_back(i);
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,i);
			}
		}
		else
		{
			Recv(vec,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(9,(int)vec.size());
		for(int i=0;i<(int)vec.size();i++)
		{
			LONGS_EQUAL(i,vec[i]);
		}
	}

	// test Send for vector of floats (size not known by receiver)
	{
		std::vector<float> vec;
		float junk = 1.7e-5;
		if(Rank() == 0)
		{
			for(int i=0;i<9;i++)
				vec.push_back(junk+(float)i);
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,i);
			}
		}
		else
		{
			Recv(vec,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(9,(int)vec.size());
		for(int i=0;i<(int)vec.size();i++)
		{
			DOUBLES_EQUAL(junk+(float)i,vec[i],MPI_FLOAT_TOL);
		}
	}

	// test Send for vector of doubles (size not known by receiver)
	{
		std::vector<double> vec;
		double junk = 1.7e-13;
		if(Rank() == 0)
		{
			for(int i=0;i<9;i++)
				vec.push_back(junk+(double)i);
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,i);
			}
		}
		else
		{
			Recv(vec,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(9,(int)vec.size());
		for(int i=0;i<(int)vec.size();i++)
		{
			DOUBLES_EQUAL(junk+(double)i,vec[i],MPI_FLOAT_TOL);
		}
	}
	
	// test Send for empty vector of integers (size not known by receiver)
	{
		std::vector<int> vec;
		if(Rank() == 0)
		{
			for(int i=1;i<NumberOfProcesses();i++)
			{
				Send(vec,i);
			}
		}
		else
		{
			Recv(vec,0);
		}
		// check that everyone has the right size vector
		LONGS_EQUAL(0,(int)vec.size());
	}

	// test Send for vector of vector of integers
	{
		vector<vector<int>> vec;
		if(Rank() == 0)
		{
			vector<int> a = {1,2,3};
			vec.push_back(a);
			vec.push_back(a);
			for(int i=1;i<NumberOfProcesses();i++)
				Send(vec,i);
		}
		else
		{
			Recv(vec,0);
			LONGS_EQUAL(2,vec.size());
			for(auto row:vec)
			{
				LONGS_EQUAL(3,row.size());
			}
		}
	}
	// test Gather() for integers
	{
		int root = 0,value=Rank();
		std::vector<int> vec;
		Gather(value,vec,root);
		// check that they were communicated properly to the root
		if(Rank() == root) {
			int nproc = NumberOfProcesses();
			LONGS_EQUAL(nproc,(int)vec.size());
			for(int i=0;i<nproc;i++)
				LONGS_EQUAL(i,vec[i]);
		}
	}
#if 1

	{	
		// test Gather() for floats
		int root = 0;
		float junk = 1.7e-12;
		float value= junk + (float) Rank();
		std::vector<float> vec;
		Gather(value,vec,root);
		// check that they were communicated properly to the root
		if(Rank() == root)
		{
			int nproc = NumberOfProcesses();
			LONGS_EQUAL(nproc,(int)vec.size());
			for(int i=0;i<nproc;i++)
				DOUBLES_EQUAL(junk+(float)i,vec[i],MPI_FLOAT_TOL);
		}
	}

	{	
		// test Gather() for doubles
		int root = 0;
		double junk = 1.7e-12;
		double value= junk + (double) Rank();
		std::vector<double> vec;
		Gather(value,vec,root);
		// check that they were communicated properly to the root
		if(Rank() == root)
		{
			int nproc = NumberOfProcesses();
			LONGS_EQUAL(nproc,(int)vec.size());
			for(int i=0;i<nproc;i++)
				DOUBLES_EQUAL(junk+(double)i,vec[i],MPI_DOUBLE_TOL);
		}
	}

	// test AllGather() for integers
	{
		int root = 0,value=Rank();
		std::vector<int> vec;
		AllGather(value,vec);
		// check that they were communicated properly to everyone
		int nproc = NumberOfProcesses();
		LONGS_EQUAL(nproc,(int)vec.size());
		for(int i=0;i<nproc;i++)
			LONGS_EQUAL(i,vec[i]);
	}
	
	// test AllGather() for floats
	{
		int root = 0;
		float junk = 1.7e-5;
		float value=junk + (float)Rank();
		std::vector<float> vec;
		AllGather(value,vec);
		// check that they were communicated properly to everyone
		int nproc = NumberOfProcesses();
		LONGS_EQUAL(nproc,(int)vec.size());
		for(int i=0;i<nproc;i++)
			DOUBLES_EQUAL(junk+(float)i,vec[i],MPI_FLOAT_TOL);
	}
	
	// test AllGather() for doubles
	{
		int root = 0;
		double junk = 1.7e-13;
		double value=junk + (double)Rank();
		std::vector<double> vec;
		AllGather(value,vec);
		// check that they were communicated properly to everyone
		int nproc = NumberOfProcesses();
		LONGS_EQUAL(nproc,(int)vec.size());
		for(int i=0;i<nproc;i++)
			DOUBLES_EQUAL(junk+(double)i,vec[i],MPI_DOUBLE_TOL);
	}

	
	// test Gather() for vector of integers
	{
		int root = 0;
		std::vector<int> send_vec;
		send_vec.push_back(Rank());
		send_vec.push_back(Rank()+1);
		std::vector<int> recv_vec;
		Gather(send_vec,2,recv_vec,root);
		// check that they were communicated properly to the root
		if(Rank() == root)
		{
			int nproc = NumberOfProcesses();
			LONGS_EQUAL(2*nproc,(int)recv_vec.size());
			for(int i=0;i<nproc;i++)
			{
				LONGS_EQUAL(i,recv_vec[2*i]);
				LONGS_EQUAL(i+1,recv_vec[2*i+1]);
			}
		}
	}
	
	// test Gather() for vector of floats
	{
		int root = 0;
		float junk = 1.7e-5;
		std::vector<float> send_vec;
		send_vec.push_back(junk + (float)Rank());
		send_vec.push_back(junk + (float)(Rank()+1));
		std::vector<float> recv_vec;
		Gather(send_vec,2,recv_vec,root);
		// check that they were communicated properly to the root
		if(Rank() == root)
		{
			int nproc = NumberOfProcesses();
			LONGS_EQUAL(2*nproc,(int)recv_vec.size());
			for(int i=0;i<nproc;i++)
			{
				DOUBLES_EQUAL(junk+(float)i,recv_vec[2*i],MPI_FLOAT_TOL);
				DOUBLES_EQUAL(junk+(float)(i+1),recv_vec[2*i+1],MPI_FLOAT_TOL);
			}
		}
	}
	
	// test Gather() for vector of doubles
	{
		int root = 0;
		double junk = 1.7e-13;
		std::vector<double> send_vec;
		send_vec.push_back(junk + (double)Rank());
		send_vec.push_back(junk + (double)(Rank()+1));
		std::vector<double> recv_vec;
		Gather(send_vec,2,recv_vec,root);
		// check that they were communicated properly to the root
		if(Rank() == root)
		{
			int nproc = NumberOfProcesses();
			LONGS_EQUAL(2*nproc,(int)recv_vec.size());
			for(int i=0;i<nproc;i++)
			{
				DOUBLES_EQUAL(junk+(double)i,recv_vec[2*i],MPI_DOUBLE_TOL);
				DOUBLES_EQUAL(junk+(double)(i+1),recv_vec[2*i+1],MPI_DOUBLE_TOL);
			}
		}
	}

	{
		// test Broadcast for integers
		int root = 0,val=0;
		if(Rank() == root)
			val = 5;
		Broadcast(val,root);
		LONGS_EQUAL(5,val);
	}
	
	{
		// test Broadcast for floats
		int root = 0;
		float val=0,check_val=5.0 + 1.7e-5;
		if(Rank() == root)
			val = check_val;
		Broadcast(val,root);
		LONGS_EQUAL(check_val,val);
	}
	
	{
		// test Broadcast for doubles
		int root = 0;
		double val=0,check_val=5.0 + 1.7e-13;
		if(Rank() == root)
			val = check_val;
		Broadcast(val,root);
		LONGS_EQUAL(check_val,val);
	}

	{
		// test Broadcast for vector of integers (known size)
		int root = 0,val=0;
		std::vector<int> vec(5,0);
		if(Rank() == root)
			for(int i=0;i<5;i++)
				vec[i] = i;
		Broadcast(vec,5,root);
		LONGS_EQUAL(5,vec.size()); // all procs have same vec length
		for(int i=0;i<5;i++)
			LONGS_EQUAL(i,vec[i]); 
	}
	
	{
		// test Broadcast for vector of integers (unknown size)
		int root = 0,val=0;
		std::vector<int> vec;
		if(Rank() == root)
			for(int i=0;i<5;i++)
				vec.push_back(i);
		Broadcast(vec,root);
		LONGS_EQUAL(5,vec.size()); // all procs have same vec length
		for(int i=0;i<5;i++)
			LONGS_EQUAL(i,vec[i]); 
	}
	
	{
		// test Broadcast for vector of floats
		int root = 0;
		float val=0,junk=1.7e-3;
		std::vector<float> vec;
		if(Rank() == root)
			for(int i=0;i<5;i++)
				vec.push_back(junk+(float)i);
		Broadcast(vec,root);
		LONGS_EQUAL(5,vec.size()); // all procs have same vec length
		for(int i=0;i<5;i++)
			DOUBLES_EQUAL(junk+(float)i,vec[i],MPI_FLOAT_TOL); 
	}
	
	{
		// test Broadcast for vector of doubles
		int root = 0;
		double val=0,junk=1.7e-14;
		std::vector<double> vec;
		if(Rank() == root)
			for(int i=0;i<5;i++)
				vec.push_back(junk+(float)i);
		Broadcast(vec,root);
		LONGS_EQUAL(5,vec.size()); // all procs have same vec length
		for(int i=0;i<5;i++)
			DOUBLES_EQUAL(junk+(double)i,vec[i],MPI_DOUBLE_TOL); 
	}

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
		int rank = MessagePasser::Rank();
		auto biggestRank = MessagePasser::ParallelSum(rank);
		LONGS_EQUAL(MessagePasser::NumberOfProcesses()-1, biggestRank);
	}
#endif
}



