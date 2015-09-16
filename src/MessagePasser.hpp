#include "checkpoint.h"
#ifdef PARFAIT_WITH_MPI

inline void MessagePasser::Init(){
	int initialized=0;
	MPI_Initialized(&initialized);
	if(!initialized)
		MPI_Init(NULL,NULL);
}

inline void MessagePasser::InitWithThreads(){
	int initialized=0;
	MPI_Initialized(&initialized);
	int provided;
	if(!initialized)
		MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
	if(provided != MPI_THREAD_MULTIPLE)
		throw std::logic_error("Could not instantiate MPI runtime with MPI_THREAD_MULTIPLE");
}


inline void MessagePasser::Finalize(){
	int finalized=0;
	MPI_Finalized(&finalized);
	if(!finalized)
		MPI_Finalize();
}

inline int MessagePasser::Rank(){
	int rank=0;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank;
}

inline int MessagePasser::NumberOfProcesses(){
	int size=0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	return size;
}

template <typename T>
inline void MessagePasser::Send(const T& value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}

template <typename T>
void MessagePasser::Recv(T &value, int source){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),source,0,MPI_COMM_WORLD,&status);
}
template <typename T>
void MessagePasser::Recv(T &value){
	MPI_Status status;
	MPI_Recv(&value,1,Type(value),MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
}
#endif

