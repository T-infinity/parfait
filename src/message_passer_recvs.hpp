
	// recv a vector (receiver knows length)
	template<typename T>
	void Recv(std::vector<T> &vec,int length,int source)
	{
		vec.clear();
		vec.assign(length,0);
		MPI_Status status;
		MPI_Recv(&vec[0],length,Type(T()),source,0,MPI_COMM_WORLD,&status);
	}
	
	// send a vector (receiver doesn't know length)
	template<typename T>
	void Send(std::vector<T> &vec,int destination)
	{
		int length = (int)vec.size();
		Send(length,destination);
		Send(vec,length,destination);
	}

	// recv a vector (receiver doesn't know length)
	template<typename T>
	void Recv(std::vector<T> &vec,int source)
	{
		vec.clear();
		int length=0;
		Recv(length,source);
		Recv(vec,length,source);
	}
