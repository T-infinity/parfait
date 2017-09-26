
template<typename T>
void MessagePasser::Send(const std::vector<T>& vec, int length, int destination) const {
    MPI_Send(vec.data(), length * sizeof(T), MPI_CHAR, destination, 0, getCommunicator());
}

template<typename T>
void MessagePasser::Send(const std::vector<T>& vec, int destination) const {
    int length = (int) vec.size();
    Send(vec, length, destination);
}

template<typename T>
void MessagePasser::Send(const std::vector<std::vector<T>>& vec, int destination) const {
    // pack into a contiguous buffer
    std::vector<T> sendBuffer;
    std::vector<int> sendBufferMap = {0};
    for (auto& row:vec)
        sendBufferMap.push_back(row.size() + sendBufferMap.back());
    sendBuffer.reserve(sendBufferMap.back());
    for (auto& row:vec)
        for (auto val:row)
            sendBuffer.push_back(val);
    Send(sendBufferMap, destination);
    Send(sendBuffer, destination);
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingSend(const T& value, int destination) const {
    MessageStatus status;
    MPI_Isend(&value, sizeof(T), MPI_CHAR, destination, 0, getCommunicator(), status.request());
    return status;
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingSend(
        const std::vector<T>& vec,
        int length, int destination) const {
    MessageStatus status;
    MPI_Isend(vec.data(), length * sizeof(T), MPI_CHAR, destination, 0, getCommunicator(), status.request());
    return status;
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingSend(
        const std::vector<T>& vec, int destination) const {
    return NonBlockingSend(vec, vec.size(), destination);
}
