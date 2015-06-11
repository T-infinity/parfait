
// ----------- Reduction operations ----------------------------------
	// sum single values to the root
	template<typename T>
	T ParallelSum(T value,int rootId)
	{
		T sum = 0;
		MPI_Reduce(&value,&sum,1,Type(value),MPI_SUM,rootId,MPI_COMM_WORLD);
		return sum;
	}

	template<typename T>
	T ParallelMax(T value,int rootId)
	{
		T tmp = value;
		T max = 0;
		MPI_Reduce(&tmp,&max,1,Type(value),MPI_MAX,rootId,MPI_COMM_WORLD);
		return max;
	}		

	template<typename T>
	std::vector<T> ParallelMax(const std::vector<T> &vec,int rootId)
	{
		std::vector<T> result(vec.size(),0);
		if(vec.size() > 0)
			MPI_Reduce((void*)vec.data(),result.data(),
					vec.size(),Type(T()),MPI_MAX,rootId,MPI_COMM_WORLD);
		return result;
	}
	
	template<typename T>
	std::vector<T> ParallelMin(const std::vector<T> &vec,int rootId)
	{
		std::vector<T> result(vec.size(),0);
		if(vec.size() > 0)
			MPI_Reduce((void*)vec.data(),result.data(),
					vec.size(),Type(T()),MPI_MIN,rootId,MPI_COMM_WORLD);
		return result;
	}
