
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
	
	// send a vector of vectors
	template<typename T>
	void Send(std::vector<std::vector<T>> &vec,int destination)
	{
		// pack into a contiguous buffer
		std::vector<T> sendBuffer;
		std::vector<int> sendBufferMap = {0};
		for(auto &row:vec)
			sendBufferMap.push_back(row.size()+sendBufferMap.back());	
		sendBuffer.reserve(sendBufferMap.back());
		for(auto &row:vec)
			for(auto val:row)
				sendBuffer.push_back(val);
		// send map to destination
		Send(sendBufferMap,destination);
		// send buffer to destination
		Send(sendBuffer,destination);
	}
	
	// Recv a vector of vectors
	template<typename T>
	void Recv(std::vector<std::vector<T>> &vec,int source)
	{
		std::vector<int> recvBufferMap;
		std::vector<T>   recvBuffer;
		Recv(recvBufferMap,source);
		Recv(recvBuffer,source);
		vec.assign(recvBufferMap.size()-1,std::vector<T>());
		// use map to unpack buffer into vector of vectors
		for(int i=0;i<recvBufferMap.size()-1;i++)
			for(int j=recvBufferMap[i];j<recvBufferMap[i+1];j++)
				vec[i].push_back(recvBuffer[j]);
	}
