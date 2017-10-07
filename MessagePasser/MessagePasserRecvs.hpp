
#include <type_traits>

template<typename T>
void MessagePasser::Recv(std::vector<T>& vec, int length, int source) const {
    vec.clear();
    vec.resize(length);
    MPI_Status status;
    MPI_Recv(vec.data(), length * sizeof(T), MPI_CHAR, source, 0, getCommunicator(), &status);
}

template<typename T>
void MessagePasser::Recv(std::vector<T>& vec, int source) const {
    int n = 0;
    MPI_Status status;
    MPI_Probe(source, 0, getCommunicator(), &status);
    MPI_Get_count(&status, MPI_CHAR, &n);
    vec.resize(n / sizeof(T));
    MPI_Recv(vec.data(), n, MPI_CHAR, source, 0, getCommunicator(), MPI_STATUS_IGNORE);
}

template<typename T>
void MessagePasser::Recv(std::vector<std::vector<T>>& vec, int source) const {
    std::vector<int> recvBufferMap;
    std::vector<T> recvBuffer;
    Recv(recvBufferMap, source);
    Recv(recvBuffer, source);
    vec.assign(recvBufferMap.size() - 1, std::vector<T>());
    // use map to unpack buffer into vector of vectors
    for (unsigned int i = 0; i < recvBufferMap.size() - 1; i++)
        for (int j = recvBufferMap[i]; j < recvBufferMap[i + 1]; j++)
            vec[i].push_back(recvBuffer[j]);
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingRecv(
        std::vector<T>& vec, int length, int source) const {
    MessageStatus status;
    vec.resize(length);
    MPI_Irecv(vec.data(), length * sizeof(T), MPI_CHAR, source, 0, getCommunicator(), status.request());
    return status;
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingRecv(
    std::vector<T>& vec, int source) const {
    static_assert(sizeof(T) != sizeof(T), "NonBlockingRecv does not work for vectors of unknown size");
}


template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingRecv(T &d, int source) const {
    MessageStatus status;
    MPI_Irecv(&d, 1 * sizeof(T), MPI_CHAR, source, 0, getCommunicator(), status.request());
    return status;
}

