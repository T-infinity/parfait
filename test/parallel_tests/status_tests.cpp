#include "message_passer.h"
#include <mpi.h>
#include <vector>

#include "CppUTest/CommandLineTestRunner.h"
#define MPI_FLOAT_TOL 1.0e-6
#define MPI_DOUBLE_TOL 1.0e-15

using namespace MessagePasser;
using std::vector;

TEST_GROUP(MessagePasserStatusTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(MessagePasserStatusTests,CheckSendStatus)
{   
    vector<MessageStatus> statuses;
    if(Rank() == 0){
        vector<int> ones(300000);
        for(int i=0;i<NumberOfProcesses();i++)
            statuses.push_back(NonBlockingSend(ones,i));
    }
    vector<int> recv_ones;
    Recv(recv_ones,0);
    
    if(Rank() == 0){
        WaitAll(statuses);
    }

}

