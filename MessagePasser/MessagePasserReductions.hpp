#include <limits>
#include <vector>

template<typename T>
T MessagePasser::ParallelSum(T value, int rootId) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    T sum = 0;
    MPI_Reduce(&value, &sum, 1, Type(value), MPI_SUM, rootId, getCommunicator());
    return sum;
}

template<typename T>
T MessagePasser::ParallelMax(T value, int rootId) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    T tmp = value;
    T max = value;
    MPI_Reduce(&tmp, &max, 1, Type(value), MPI_MAX, rootId, getCommunicator());
    return max;
}
template<typename T>
T MessagePasser::ParallelMin(T value, int rootId) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    T tmp = value;
    T min = value;
    MPI_Reduce(&tmp, &min, 1, Type(value), MPI_MIN, rootId, getCommunicator());
    return min;
}
template<typename T>
T MessagePasser::ParallelMin(T value) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    T tmp = value;
    T min;
    MPI_Allreduce(&tmp, &min, 1, Type(value), MPI_MIN, getCommunicator());
    return min;
}

template<typename T>
T MessagePasser::ParallelMax(T value) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    T tmp = value;
    T max;
    MPI_Allreduce(&tmp, &max, 1, Type(value), MPI_MAX, getCommunicator());
    return max;
}

template<typename T>
std::vector<T> MessagePasser::ParallelMax(const std::vector<T>& vec, int rootId) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    std::vector<T> result;
    result.resize(vec.size());
    if (vec.size() > 0)
        MPI_Reduce((void*) vec.data(), result.data(),
                   vec.size(), Type(T()), MPI_MAX, rootId, getCommunicator());
    return result;
}

template<typename T>
std::vector<T> MessagePasser::ParallelMin(const std::vector<T>& vec, int rootId) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    std::vector<T> result;
    result.resize(vec.size());
    if (vec.size() > 0)
        MPI_Reduce((void*) vec.data(), result.data(),
                   vec.size(), Type(T()), MPI_MIN, rootId, getCommunicator());
    return result;
}

template<typename T>
T MessagePasser::ParallelRankOfMax(T value) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    struct {
      T value;
      int rank;
    } tmp, max;
    tmp.value = value;
    tmp.rank  = Rank();
    MPI_Reduce(&tmp, &max, 1, Type(tmp.value,tmp.rank), MPI_MAXLOC, 0, getCommunicator());
    Broadcast(max.rank, 0);
    return max.rank;
}

template<typename T>
T MessagePasser::ParallelSum(const T& value) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    auto sum = ParallelSum(value, 0);
    Broadcast(sum, 0);
    return sum;
}

template<typename T>
std::vector<T> MessagePasser::ElementalMax(std::vector<T>& vec, int root) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    auto result = vec;
    std::vector<T> recv_vec(vec.size());
    NonBlockingSend(vec, vec.size(), root);
    if (Rank() == root) {
        for (int i = 0; i < NumberOfProcesses(); ++i) {
            Recv(recv_vec, int(vec.size()), i);
            for (auto j = 0u; j < vec.size(); ++j)
                result[j] = std::max(recv_vec[j], result[j]);
        }
    }
    Barrier();
    return result;
}

template<typename T>
std::vector<T> MessagePasser::AllElementalMax(std::vector<T>& vec) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    auto result = ElementalMax(vec, 0);
    Broadcast(result, 0);
    return result;
}
