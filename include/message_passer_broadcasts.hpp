
	// broadcast a value
	template<typename T>
	void Broadcast(T &value,int rootId)
	{
		MPI_Bcast(&value,1,Type(value),rootId,MPI_COMM_WORLD);
	}

	// broadcast a vector of KNOWN size
	template<typename T>
	void Broadcast(std::vector<T> &vec,int vecLength,int rootId)
	{
		T t;
		if(vec.size() != vecLength)
		{
			fprintf(stderr,"MessagePasser::Broadcast: Target vector is not the ");
			fprintf(stderr,"same size as the broadcast message\n");
			fprintf(stderr,"-----------Rank: %i   Vector length: %i   Message size: %i\n",vec.size(),vecLength);
			exit(0);
		}
		if(vecLength != 0)
			MPI_Bcast(&vec[0],vecLength,Type(t),rootId,MPI_COMM_WORLD);
	}

	// broadcast a vector of unknown size
	template<typename T>
	void Broadcast(std::vector<T> &vec,int rootId)
	{
		int size = 0;
		if(Rank() == rootId)
			size = (int)vec.size();
		Broadcast(size,rootId);
		if(Rank() != rootId)
		{
			vec.clear();
			vec.assign(size,0);
		}
		T t;
		MPI_Bcast(&vec[0],size,Type(t),rootId,MPI_COMM_WORLD);
	}
