
template<typename T>
void MessagePasser::Gather(T value,std::vector<T> &vec,int rootId) {
	vec.resize(NumberOfProcesses());
	MPI_Gather(&value,sizeof(T),MPI_CHAR,vec.data(),sizeof(T),MPI_CHAR,rootId,MPI_COMM_WORLD);
}

template<typename T>
void MessagePasser::AllGather(T value,std::vector<T> &vec) {
	vec.assign(NumberOfProcesses(),0);
	MPI_Allgather(&value,sizeof(T),MPI_CHAR,vec.data(),sizeof(T),MPI_CHAR,MPI_COMM_WORLD);
}

template<typename T>
void MessagePasser::Gather(const std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId) {
	if(Rank() == rootId) {
		recv_vec.clear();
		recv_vec.assign(send_count*NumberOfProcesses(),0);
	}
	MPI_Gather(send_vec.data(),send_count*sizeof(T),MPI_CHAR,recv_vec.data(),send_count*sizeof(T),
			MPI_CHAR,rootId,MPI_COMM_WORLD);
}

template<typename T>
void MessagePasser::Gatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec,
		std::vector<int> &map,int rootId) {
	int sendcount = (int)send_vec.size()*sizeof(T);
	int nproc = NumberOfProcesses();
	std::vector<int> recv_counts(nproc,0);
	Gather(sendcount,recv_counts,rootId);
	if(Rank() == rootId) {
		map.assign(nproc+1,0);
		for(int i=1;i<nproc+1;i++)
			map[i] = map[i-1] + recv_counts[i-1];
		recv_vec.assign(map.back()/sizeof(T),0);
	}
	MPI_Gatherv(send_vec.data(),sendcount,MPI_CHAR,
			recv_vec.data(),recv_counts.data(),map.data(),MPI_CHAR,
			rootId,MPI_COMM_WORLD);
	// make map make sense for original datatype
	for(auto& x:map)
		x /= sizeof(T);
}

template<typename T>
void MessagePasser::Gatherv(const std::vector<T>& send_vec,
							std::vector<std::vector<T>>& result,int root_id){
	std::vector<int> map;
	std::vector<T> recv;
	if(Rank() == root_id)
		result.assign(NumberOfProcesses(),std::vector<T>());
	Gatherv(send_vec,recv,map,root_id);
	if(Rank() == root_id) {
		for (int i = 0; i < NumberOfProcesses(); i++) {
			for(int j=map[i];j<map[i+1];j++)
				result[i].push_back(recv[j]);
		}
	}
}

template<typename T>
void MessagePasser::Gatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec,
		int rootId){
    std::vector<int> m;
    Gatherv(send_vec,recv_vec,m,rootId);
}

template<typename T>
void MessagePasser::AllGatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec){
	std::vector<int> m;
	AllGatherv(send_vec,recv_vec,m);
}

template <typename T>
void MessagePasser::AllGatherv(const std::vector<T>& send_vec,
																std::vector<std::vector<T>>& vec_of_vec_output){
	std::vector<int> m;
	std::vector<T> recv_vec;
	AllGatherv(send_vec,recv_vec,m);
	for(int i=0;i<m.size()-1;i++){
		int n = m[i+1] - m[i];
		vec_of_vec_output.push_back(std::vector<T>(n,0));
		for(int j=0;j<n;j++){
			vec_of_vec_output[i][j] = recv_vec[m[i]+j];
		}
	}
}


template<typename T>
void MessagePasser::AllGatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec,std::vector<int> &map) {
	int sendcount = (int)send_vec.size()*sizeof(T);
	int nproc = NumberOfProcesses();
	std::vector<int> recv_counts(nproc,0);
	AllGather(sendcount,recv_counts);
	map.assign(nproc+1,0);
	for(int i=1;i<nproc+1;i++)
		map[i] = map[i-1] + recv_counts[i-1];
	recv_vec.clear();
	recv_vec.assign(map.back()/sizeof(T),0);
	MPI_Allgatherv(&send_vec[0],sendcount,MPI_CHAR,
			&recv_vec[0],&recv_counts[0],&map[0],MPI_CHAR,
			MPI_COMM_WORLD);
	for(auto& x:map)
		x /= sizeof(T);
}
