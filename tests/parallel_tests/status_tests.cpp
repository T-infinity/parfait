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
    vector<int> recv_ones;
    MessageStatus recv_status = NonBlockingRecv(recv_ones,30000,0);
    
    vector<MessageStatus> statuses;
    if(Rank() == 0){
        vector<int> ones(30000,1);
        for(int i=0;i<NumberOfProcesses();i++)
            statuses.push_back(NonBlockingSend(ones,i));
    }
    
    Wait(recv_status);

    LONGS_EQUAL(30000,recv_ones.size());
    LONGS_EQUAL(1,recv_ones[0]);

    if(Rank() == 0){
        WaitAll(statuses);
    }

}

