namespace MessagePasser {
  template<typename T>
  void Send(std::vector<T> &vec, int length, int destination) {
      MPI_Send(vec.data(), length, Type(T()), destination, 0, MPI_COMM_WORLD);
  }

// send a vector (receiver doesn't know length)
  template<typename T>
  void Send(std::vector<T> &vec, int destination) {
      int length = (int) vec.size();
      Send(vec, length, destination);
  }

// send a vector of vectors
  template<typename T>
  void Send(std::vector<std::vector<T>> &vec, int destination) {
      // pack into a contiguous buffer
      std::vector<T> sendBuffer;
      std::vector<int> sendBufferMap = {0};
      for (auto &row:vec)
          sendBufferMap.push_back(row.size() + sendBufferMap.back());
      sendBuffer.reserve(sendBufferMap.back());
      for (auto &row:vec)
          for (auto val:row)
              sendBuffer.push_back(val);
      // send map to destination
      Send(sendBufferMap, destination);
      // send buffer to destination
      Send(sendBuffer, destination);
  }

// ready send a vector
  template<typename T>
  MessageStatus NonBlockingSend(std::vector<T> &vec, int length, int destination) {
      MessageStatus status;
      MPI_Isend(vec.data(), length, Type(T()), destination, 0, MPI_COMM_WORLD, status.request());
      return status;
  }

  template<typename T>
  MessageStatus NonBlockingSend(std::vector<T> &vec, int destination) {
      return NonBlockingSend(vec, vec.size(), destination);
  }
}
