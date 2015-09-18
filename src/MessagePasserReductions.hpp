namespace MessagePasser {
  template<typename T>
  T ParallelSum(T value, int rootId) {
	  T sum = 0;
	  MPI_Reduce(&value, &sum, 1, Type(value), MPI_SUM, rootId, MPI_COMM_WORLD);
	  return sum;
  }

  template<typename T>
  T ParallelMax(T value, int rootId) {
	  T tmp = value;
	  T max = value;
	  MPI_Reduce(&tmp, &max, 1, Type(value), MPI_MAX, rootId, MPI_COMM_WORLD);
	  return max;
  }
  template<typename T>
  T ParallelMin(T value, int rootId) {
	  T tmp = value;
	  T min = value;
	  MPI_Reduce(&tmp, &min, 1, Type(value), MPI_MIN, rootId, MPI_COMM_WORLD);
	  return min;
  }
  template <typename T>
  T ParallelMinAll(T value) {
	  T tmp = value;
	  T min;
	  MPI_Allreduce(&tmp, &min, 1, Type(value), MPI_MIN, MPI_COMM_WORLD);
	  return min;
  }

  template <typename T>
  T ParallelMaxAll(T value) {
	  T tmp = value;
	  T max;
	  MPI_Allreduce(&tmp, &max, 1, Type(value), MPI_MAX, MPI_COMM_WORLD);
	  return max;
  }

  template<typename T>
  std::vector<T> ParallelMax(const std::vector<T> &vec, int rootId) {
	  std::vector<T> result;
          result.resize(vec.size());
	  if (vec.size() > 0)
		  MPI_Reduce((void *) vec.data(), result.data(),
					 vec.size(), Type(T()), MPI_MAX, rootId, MPI_COMM_WORLD);
	  return result;
  }

  template<typename T>
  std::vector<T> ParallelMin(const std::vector<T> &vec, int rootId) {
	  std::vector<T> result;
          result.resize(vec.size());
	  if (vec.size() > 0)
		  MPI_Reduce((void *) vec.data(), result.data(),
					 vec.size(), Type(T()), MPI_MIN, rootId, MPI_COMM_WORLD);
	  return result;
  }

  template<typename T>
  T ParallelSum(const T &value) {
	  auto sum = ParallelSum(value, 0);
	  Broadcast(sum, 0);
	  return sum;
  }
}
