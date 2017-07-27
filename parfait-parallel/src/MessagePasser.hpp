#include <stdexcept>

inline void MessagePasser::Init() {
    int initialized = 0;
    MPI_Initialized(&initialized);
    if (not initialized)
        MPI_Init(NULL, NULL);
    const char* comm_name = "MPI_COMM_WORLD";
    MPI_Comm_set_name(MPI_COMM_WORLD, comm_name);
}

inline void MessagePasser::InitWithThreads() {
    int initialized = 0;
    MPI_Initialized(&initialized);
    int provided;
    if (not initialized)
        MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE)
        throw std::logic_error("Could not instantiate MPI runtime with MPI_THREAD_MULTIPLE");
    const char* comm_name = "MPI_COMM_WORLD";
    MPI_Comm_set_name(MPI_COMM_WORLD, comm_name);
}

inline void MessagePasser::Finalize() {
    int initialized, finalized;
    MPI_Initialized(&initialized);
    if(not initialized) return;
    MPI_Finalized(&finalized);
    if (not finalized) {
        MPI_Finalize();
    }
}

inline int MessagePasser::Rank() const {
    int rank = 0;
    MPI_Comm_rank(getCommunicator(), &rank);
    return rank;
}

inline int MessagePasser::NumberOfProcesses() const {
    int size = 0;
    MPI_Comm_size(getCommunicator(), &size);
    return size;
}

template<typename T>
void MessagePasser::Send(const T& value, int destination) const {
    MPI_Send(&value, 1, Type(value), destination, 0, getCommunicator());
}

template<typename T>
void MessagePasser::Recv(T& value, int source) const {
    MPI_Status status;
    MPI_Recv(&value, 1, Type(value), source, 0, getCommunicator(), &status);
}
template<typename T>
void MessagePasser::Recv(T& value) const {
    MPI_Status status;
    MPI_Recv(&value, 1, Type(value), MPI_ANY_SOURCE, 0, getCommunicator(), &status);
}
