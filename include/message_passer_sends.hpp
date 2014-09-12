
	//Send a single value
	template<typename T>
	void Send(T value,int destination)
	{
		MPI_Send(&value,1,Type(value),destination,0,MPI_COMM_WORLD);
	}
	
	// recv a single value
	template<typename T>	
	void Recv(T &value,int source)
	{
		MPI_Status status;
		MPI_Recv(&value,1,Type(value),source,0,MPI_COMM_WORLD,&status);
	}

	// recv a single value from any source
	template<typename T>	
	void Recv(T &value)
	{
		MPI_Status status;
		MPI_Recv(&value,1,Type(value),MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
	}

	// send a vector (receiver knows length)
	template<typename T>
	void Send(std::vector<T> &vec,int length,int destination)
	{
		T t;
		MPI_Send(&vec[0],length,Type(t),destination,0,MPI_COMM_WORLD);
	}
