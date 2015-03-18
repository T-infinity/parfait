
	


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
