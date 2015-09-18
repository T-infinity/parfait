namespace MessagePasser {
// scatter a vector that has exactly nproc 	elements
  template<typename T>
  void Scatter(std::vector<T> &vec, T &recv_value, int rootId) {
	  MPI_Scatter(vec.data(), sizeof(T), MPI_CHAR, &recv_value, sizeof(T), MPI_CHAR, rootId, MPI_COMM_WORLD);
  }

// scatter a vector that divides evenly into the number of procs
  template<typename T>
  void Scatter(std::vector<T> &vec, std::vector<T> &recv_vec, int rootId) {
	  int size = 0;
	  if (Rank() == rootId) {
		  int total_length = (int) vec.size();
		  assert(total_length % NumberOfProcesses() == 0); // must scatter equal amounts
		  size = total_length / NumberOfProcesses();
	  }
	  Broadcast(size, rootId);
	  recv_vec.assign(size, 0);
	  MPI_Scatter(vec.data(), size*sizeof(T), MPI_CHAR, recv_vec.data(), size*sizeof(T), MPI_CHAR, rootId, MPI_COMM_WORLD);
  }

// scatterv a vector that does not divide evenly into number of procs
  template<typename T>
  void Scatterv(std::vector<T> &vec, std::vector<T> &recv_vec, int rootId) {
	  std::vector<int> sendcounts;
	  std::vector<int> displs;
	  int local_size = 0;
	  if (Rank() == rootId) {
		  int total_size = (int) vec.size();
		  int nproc = NumberOfProcesses();
          sendcounts.assign(nproc,total_size / nproc);
		  for (int i = 0; i < nproc; i++)
			  if (i < (total_size % nproc))
				  sendcounts[i]++;
		  displs.assign(nproc, 0);
		  for (int i = 1; i < nproc; i++)
			  displs[i] = displs[i - 1] + sendcounts[i - 1];
	  }
	  Scatter(sendcounts, local_size, rootId);
	  recv_vec.assign(local_size, 0);
        //convert sendcounts etc to char numbering
        for(auto& x:sendcounts)
            x *= sizeof(T);
        for(auto& x:displs)
            x *= sizeof(T);
        local_size *= sizeof(T);
	  MPI_Scatterv(vec.data(), sendcounts.data(), displs.data(), MPI_CHAR,
				   recv_vec.data(), local_size, MPI_CHAR, rootId, MPI_COMM_WORLD);
  }
}