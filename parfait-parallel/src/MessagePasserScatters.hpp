template<typename T>
void MessagePasser::Scatter(std::vector<T>& vec, T& recv_value, int rootId) const {
    MPI_Scatter(vec.data(), sizeof(T), MPI_CHAR, &recv_value, sizeof(T), MPI_CHAR, rootId, getCommunicator());
}

template<typename T>
void MessagePasser::Scatter(std::vector<T>& vec, std::vector<T>& recv_vec, int rootId) const {
    int size = 0;
    if (Rank() == rootId) {
        int total_length = (int) vec.size();
        assert(total_length % NumberOfProcesses() == 0); // must scatter equal amounts
        size = total_length / NumberOfProcesses();
    }
    Broadcast(size, rootId);
    recv_vec.resize(size);
    MPI_Scatter(vec.data(), size * sizeof(T), MPI_CHAR, recv_vec.data(), size * sizeof(T), MPI_CHAR, rootId,
                getCommunicator());
}

template<typename T>
void MessagePasser::Scatterv(std::vector<T>& vec, std::vector<T>& recv_vec, int rootId) const {
    std::vector<int> sendcounts;
    std::vector<int> displs;
    int local_size = 0;
    if (Rank() == rootId) {
        int total_size = (int) vec.size();
        int nproc = NumberOfProcesses();
        sendcounts.assign(nproc, total_size / nproc);
        for (int i = 0; i < nproc; i++)
            if (i < (total_size % nproc))
                sendcounts[i]++;
        displs.assign(nproc, 0);
        for (int i = 1; i < nproc; i++)
            displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
    Scatter(sendcounts, local_size, rootId);
    recv_vec.resize(local_size);
    //convert sendcounts etc to char numbering
    for (auto& x:sendcounts)
        x *= sizeof(T);
    for (auto& x:displs)
        x *= sizeof(T);
    local_size *= sizeof(T);
    MPI_Scatterv(vec.data(), sendcounts.data(), displs.data(), MPI_CHAR,
                 recv_vec.data(), local_size, MPI_CHAR, rootId, getCommunicator());
}
