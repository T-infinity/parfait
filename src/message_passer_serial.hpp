namespace MessagePasser
{
	// Basic utility functions
	inline void Init() {}
	inline void Finalize() {}
	inline int Rank() {return 0;}
	inline int NumberOfProcesses() {return 1;}
	inline void Barrier() {}
	
	//-------------- point to point communication ---------------------

	inline void Send(double value,int destination){}
	inline void Send(float value,int destination){}
	inline void Send(int value,int destination){}

	inline void Recv(double &value,int source){}
	inline void Recv(float &value,int source){}
	inline void Recv(int &value,int source){}

	inline void Recv(double &value){}
	void Recv(float &value){}
	void Recv(int &value){}

	// send a vector (receiver knows length)
	template<typename T>
	void Send(std::vector<T> &vec,int length,int destination){}

	// recv a vector (receiver knows length)
	template<typename T>
	void Recv(std::vector<T> &vec,int length,int source){}
	
	// send a vector (receiver doesn't know length)
	template<typename T>
	void Send(std::vector<T> &vec,int destination){}
	
	// send a vector of vectors
	template<typename T>
	void Send(std::vector<std::vector<T>> &vec,int destination){}

	// recv a vector (receiver doesn't know length)
	template<typename T>
	void Recv(std::vector<T> &vec,int source){}
	
	// recv a vector of vectors
	template<typename T>
	void Recv(std::vector<std::vector<T>> &vec,int source){}

	// -------------  collective communication -----------------------
	// Gather values to a vector on the root
	template<typename T>
	void Gather(T value,std::vector<T> &vec,int rootId)
	{
		vec.clear();
		vec.push_back(value);
	}
	
	// Gather values to a vector on the root
	template<typename T>
	void AllGather(T value,std::vector<T> &vec)
	{
		vec.clear();
		vec.push_back(value);
	}

	// Gather vectors of a given size to the root
	template<typename T>
	void Gather(std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId)
	{
		recv_vec = send_vec;
	}

	// Gatherv vectors of different lengths to the root
	template<typename T>
	void Gatherv(std::vector<T> &send_vec,std::vector<T> &recv_vec,
			std::vector<int> &map,int rootId)
	{
		recv_vec = send_vec;
		map.clear();
		map.push_back(0);
		map.push_back((int)send_vec.size());
	}

	// broadcast a value
	template<typename T>
	void Broadcast(T &value,int rootId){}
	
	// broadcast a vector of KNOWN size
	template<typename T>
	void Broadcast(std::vector<T> &vec,int vecLength,int rootId){}

	// broadcast a vector of unknown size
	template<typename T>
	void Broadcast(std::vector<T> &vec,int rootId){}

	// scatter a vector that has exactly nproc 	elements
	template<typename T>
	void Scatter(std::vector<T> &vec,T &recv_value,int rootId)
	{
		recv_value = vec[0];
	}

	// scatter a vector that divides evenly into the number of procs
	template<typename T>
	void Scatter(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId)
	{
		recv_vec = vec;
	}

	// scatterv a vector that does not divide evenly into number of procs
	template<typename T>
	void Scatterv(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId)
	{
		recv_vec = vec;
	}

	// ----------- Reduction operations ----------------------------------
	// sum single values to the root
	template<typename T>
	T ParallelSum(T value,int rootid){return value;}
	
	// calculate max value across procs
	template<typename T>
	T ParallelMax(T value,int rootId){return value;}
	
	// get the max of values in a vector
	template<typename T>
	std::vector<T> ParallelMax(const std::vector<T> &vec,int rootId){return vec;}
}
