#ifdef PARFAIT_WITH_MPI
#include "message_passer.h"

void MessagePasser::Init(){
	int initialized=0;
	MPI_Initialized(&initialized);
	if(!initialized)
		MPI_Init(NULL,NULL);
}
void MessagePasser::Finalize(){
	int finalized=0;
	MPI_Finalized(&finalized);
	if(!finalized)
		MPI_Finalize();
}

int MessagePasser::Rank(){
	int rank=0;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank;
}

int MessagePasser::NumberOfProcesses(){
	int size=0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	return size;
}

//Send a single value
void MessagePasser::Send(double value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}
void MessagePasser::Send(float value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}
void MessagePasser::Send(int value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}



#endif

