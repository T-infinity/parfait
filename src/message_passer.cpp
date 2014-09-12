#ifdef PARFAIT_WITH_MPI
#include "message_passer.h"

void MessagePasser::Init()
{
	int initialized=0;
	MPI_Initialized(&initialized);
	if(!initialized)
		MPI_Init(NULL,NULL);
}
void MessagePasser::Finalize()
{
	int finalized=0;
	MPI_Finalized(&finalized);
	if(!finalized)
		MPI_Finalize();
}

int MessagePasser::Rank()
{
	int rank=0;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank;
}

int MessagePasser::NumberOfProcesses()
{
	int size=0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	return size;
}
#endif

