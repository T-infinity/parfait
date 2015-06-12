#ifdef PARFAIT_WITH_MPI

inline void MessagePasser::Init(){
	int initialized=0;
	MPI_Initialized(&initialized);
	if(!initialized)
		MPI_Init(NULL,NULL);
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

//Send a single value
inline void MessagePasser::Send(double value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}

inline void MessagePasser::Send(float value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}

inline void MessagePasser::Send(int value,int destination){
    MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
}


// recv a single value
inline void MessagePasser::Recv(double &value,int source){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),source,0,MPI_COMM_WORLD,&status);
}

inline void MessagePasser::Recv(float &value,int source){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),source,0,MPI_COMM_WORLD,&status);
}

inline void MessagePasser::Recv(int &value,int source){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),source,0,MPI_COMM_WORLD,&status);
}

// recv a single value from any source
inline void MessagePasser::Recv(double &value){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
}

inline void MessagePasser::Recv(float &value){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
}

inline void MessagePasser::Recv(int &value){
    MPI_Status status;
    MPI_Recv(&value,1,Type(value),MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
}

#endif

