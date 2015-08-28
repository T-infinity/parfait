
#include <type_traits>

namespace MessagePasser {
// recv a vector (receiver knows length)
  template<typename T>
  void Recv(std::vector<T> &vec, int length, int source) {
      vec.clear();
      vec.assign(length, 0);
      MPI_Status status;
      MPI_Recv(&vec[0], length, Type(T()), source, 0, MPI_COMM_WORLD, &status);
  }

// recv a vector (receiver doesn't know length)
  template<typename T>
  void Recv(std::vector<T> &vec, int source) {
      int n = 0;
      MPI_Status status;
      MPI_Probe(source, 0, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, Type(T()), &n);
      vec.assign(n, 0);
      MPI_Recv(vec.data(), n, Type(T()), source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

// Recv a vector of vectors
  template<typename T>
  void Recv(std::vector<std::vector<T>> &vec, int source) {
      std::vector<int> recvBufferMap;
      std::vector<T> recvBuffer;
      Recv(recvBufferMap, source);
      Recv(recvBuffer, source);
      vec.assign(recvBufferMap.size() - 1, std::vector<T>());
      // use map to unpack buffer into vector of vectors
      for (int i = 0; i < recvBufferMap.size() - 1; i++)
          for (int j = recvBufferMap[i]; j < recvBufferMap[i + 1]; j++)
              vec[i].push_back(recvBuffer[j]);
  }

  template<typename T>
  MessageStatus NonBlockingRecv(std::vector<T> &vec, int length, int source) {
      MessageStatus status;
      vec.assign(length, 0);
      MPI_Irecv(vec.data(), length, Type(T()), source, 0, MPI_COMM_WORLD, status.request());
      return status;
  }
}