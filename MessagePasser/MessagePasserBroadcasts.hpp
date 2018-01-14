#include <cstdlib>
#include <string>
#include <vector>

inline void MessagePasser::Broadcast(std::string& s, int rootId) const {
    std::vector<char> to_send(s.c_str(), s.c_str() + s.size());
    Broadcast(to_send, rootId);
    s = std::string(to_send.begin(), to_send.end());
}

template<typename T>
void MessagePasser::Broadcast(T& value, int rootId) const {
    MPI_Bcast(&value, sizeof(T), MPI_CHAR, rootId, getCommunicator());
}

template<typename T>
void MessagePasser::Broadcast(std::vector<T>& vec, int vecLength, int rootId) const {
    if ((int) vec.size() != vecLength) {
        if (Rank() == rootId) {
            fprintf(stderr, "MessagePasser::Broadcast: Root is trying to ");
            fprintf(stderr, "send a vector of the wrong length\n");
            fprintf(stderr, "-----------Rank: %i   Vector length: %i   Message size: %i\n",
                    Rank(), (int) vec.size(), vecLength);
            std::exit(0);
        } else
            vec.resize(vecLength);
    }
    if (vecLength != 0)
        MPI_Bcast(vec.data(), vecLength * sizeof(T), MPI_CHAR, rootId, getCommunicator());
}

template<typename T>
void MessagePasser::Broadcast(std::vector<T>& vec, int rootId) const {
    int size = 0;
    if (Rank() == rootId)
        size = (int) vec.size();
    Broadcast(size, rootId);
    if (Rank() != rootId) {
        vec.clear();
        vec.resize(size);
    }
    MPI_Bcast(vec.data(), size * sizeof(T), MPI_CHAR, rootId, getCommunicator());
}
