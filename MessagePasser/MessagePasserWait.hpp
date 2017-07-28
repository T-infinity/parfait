template<typename StatusType>
void MessagePasser::Wait(StatusType& status) const {
    MPI_Status s;
    MPI_Wait(status.request(), &s);
}

template<typename StatusType>
void MessagePasser::WaitAll(std::vector<StatusType>& statuses) const {
    for (auto& status:statuses) {
        MPI_Status s;
        MPI_Wait(status.request(), &s);
    }
}
