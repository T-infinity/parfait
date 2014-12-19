
// Gather values to a vector on the root
template<typename T>
void Gather(T value,std::vector<T> &vec,int rootId)
{
	vec.assign(NumberOfProcesses(),0);
	MPI_Gather(&value,1,Type(value),&vec[0],1,Type(value),rootId,MPI_COMM_WORLD);
}

// Gather values to a vector on every proc
template<typename T>
void AllGather(T value,std::vector<T> &vec)
{
	vec.assign(NumberOfProcesses(),0);
	MPI_Allgather(&value,1,Type(value),&vec[0],1,Type(value),MPI_COMM_WORLD);
}

// Gather vectors of a given size to the root
template<typename T>
void Gather(std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId)
{
	if(Rank() == rootId)
	{
		recv_vec.clear();
		recv_vec.assign(send_count*NumberOfProcesses(),0);
	}
	T t;	
	MPI_Gather(&send_vec[0],send_count,Type(t),&recv_vec[0],send_count,
			Type(t),rootId,MPI_COMM_WORLD);
}

// Gatherv vectors of different lengths to the root
template<typename T>
void Gatherv(std::vector<T> &send_vec,std::vector<T> &recv_vec,
		std::vector<int> &map,int rootId)
{
	int sendcount = (int)send_vec.size();	
	int nproc = NumberOfProcesses();
	std::vector<int> recv_counts(nproc,0);
	Gather(sendcount,recv_counts,rootId);
	int messageSize = 0;
	if(Rank() == rootId)
	{
		map.clear();
		map.assign(nproc+1,0);
		for(int i=1;i<nproc+1;i++)
			map[i] = map[i-1] + recv_counts[i-1];
		recv_vec.clear();
		recv_vec.assign(map.back(),0);
		messageSize = map.back();
	}
	T t;
	MPI_Gatherv(&send_vec[0],sendcount,Type(t),
			&recv_vec[0],&recv_counts[0],&map[0],Type(t),
			rootId,MPI_COMM_WORLD);	
}

template<typename T>
void AllGatherv(std::vector<T> &send_vec,std::vector<T> &recv_vec,std::vector<int> &map)
{
	int sendcount = (int)send_vec.size();	
	int nproc = NumberOfProcesses();
	std::vector<int> recv_counts(nproc,0);
	AllGather(sendcount,recv_counts);
	int messageSize = 0;
	map.clear();
	map.assign(nproc+1,0);
	for(int i=1;i<nproc+1;i++)
		map[i] = map[i-1] + recv_counts[i-1];
	recv_vec.clear();
	recv_vec.assign(map.back(),0);
	messageSize = map.back();
	T t;
	MPI_Allgatherv(&send_vec[0],sendcount,Type(t),
			&recv_vec[0],&recv_counts[0],&map[0],Type(t),
			MPI_COMM_WORLD);	
}
