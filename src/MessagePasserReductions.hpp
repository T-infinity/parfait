#include <limits>
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

	template<typename T>
	std::vector<T> ElementalMax(std::vector<T>& vec,int root){
		auto result = vec;
		std::vector<T> recv_vec(vec.size());
		NonBlockingSend(vec,vec.size(),root);
		if(Rank() == root){
			for(int i=0;i<NumberOfProcesses();++i){
				Recv(recv_vec,int(vec.size()),i);
				for(auto j=0u;j<vec.size();++j)
					result[j] = std::max(recv_vec[j],result[j]);
			}
		}
		Barrier();
		return result;
	}

	template<typename T>
	std::vector<T> AllElementalMax(std::vector<T>& vec){
		auto result = ElementalMax(vec,0);
		Broadcast(result,0);
		return result;
	}
}
