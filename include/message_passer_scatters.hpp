
// scatter a vector that has exactly nproc 	elements
template<typename T>
void Scatter(std::vector<T> &vec,T &recv_value,int rootId)
{
	MPI_Scatter(&vec[0],1,Type(recv_value),&recv_value,1,Type(recv_value),rootId,MPI_COMM_WORLD);	
}

// scatter a vector that divides evenly into the number of procs
template<typename T>
void Scatter(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId)
{
	int size = 0;
	if(Rank() == rootId)
	{
		int total_length = (int) vec.size();
		assert(total_length % NumberOfProcesses() == 0); // must scatter equal amounts
		size = total_length / NumberOfProcesses();	
	}
	Broadcast(size,rootId);
	recv_vec.clear();
	recv_vec.assign(size,0);
	T t;
	MPI_Scatter(&vec[0],size,Type(t),&recv_vec[0],size,Type(t),rootId,MPI_COMM_WORLD);	
}

// scatterv a vector that does not divide evenly into number of procs
template<typename T>
void Scatterv(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId)
{
	std::vector<int> sendcounts;
	std::vector<int> displs;
	int local_size=0;
	if(Rank() == rootId)
	{
		int total_size = (int) vec.size();
		int nproc = NumberOfProcesses();
		for(int i=0;i<nproc;i++)
			if(i<(total_size % nproc))
				sendcounts.push_back(total_size/nproc +1);
			else
				sendcounts.push_back(total_size/nproc);
		displs.assign(nproc,0);
		for(int i=1;i<nproc;i++)
			displs[i] = displs[i-1] + sendcounts[i-1];
	}
	Scatter(sendcounts,local_size,rootId);
	recv_vec.clear();
	recv_vec.assign(local_size,0);
	T t;
	MPI_Scatterv(&vec[0],&sendcounts[0],&displs[0],Type(t),
			&recv_vec[0],local_size,Type(t),rootId,MPI_COMM_WORLD);

}
