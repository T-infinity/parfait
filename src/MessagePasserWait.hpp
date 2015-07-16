template<typename StatusType>
void Wait(StatusType& status){
    MPI_Status s;
    MPI_Wait(status.request(),&s);
}

template<typename StatusType>
void WaitAll(std::vector<StatusType>& statuses){
    for(auto& status:statuses){
        MPI_Status s;
        MPI_Wait(status.request(),&s);
    }
}
