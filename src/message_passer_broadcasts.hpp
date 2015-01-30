
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
		if(vec.size() != vecLength){
			if(Rank() == rootId){
				fprintf(stderr,"MessagePasser::Broadcast: Root is trying to ");
				fprintf(stderr,"send a vector of the wrong length\n");
				fprintf(stderr,"-----------Rank: %i   Vector length: %i   Message size: %i\n",
						Rank(),vec.size(),vecLength);
				exit(0);
			}
			else
				vec.resize(vecLength);
		}
		if(vecLength != 0)
			MPI_Bcast(&vec[0],vecLength,Type(T()),rootId,MPI_COMM_WORLD);
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
		MPI_Bcast(&vec[0],size,Type(T()),rootId,MPI_COMM_WORLD);
	}
