template<typename StatusType>
void Wait(StatusType& status){
    MPI_Status s;
    MPI_Wait(status.request(),&s);
}

template<typename StatusType>
void WaitAll(std::vector<StatusType>& statuses){
    for(int i=0;i<NumberOfProcesses();i++){
        MPI_Status s;
        MPI_Wait(statuses[i].request(),&s);
    }
}
