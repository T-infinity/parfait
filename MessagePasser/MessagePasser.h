#pragma once
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <mpi.h>

class MessagePasser {
public:
    class MessageStatus {
    public:
        inline MessageStatus() : r(std::make_shared<MPI_Request>()){}
        inline MPI_Request* request() { return r.get(); }
        inline void wait() {
            MPI_Status no_one_cares;
            MPI_Wait(request(), &no_one_cares);
        }
    private:
        std::shared_ptr<MPI_Request> r;
    };

    MessagePasser();
    MessagePasser(MPI_Comm comm);
    static void Init();
    static void Finalize();
    int Rank() const;
    int NumberOfProcesses() const;
    void setCommunicator(MPI_Comm comm);
    MPI_Comm getCommunicator() const;
    inline MPI_Datatype Type(int value) const { return MPI_INT; }
    inline MPI_Datatype Type(size_t value) const { return MPI_LONG; }
    inline MPI_Datatype Type(long value) const { return MPI_LONG; }
    inline MPI_Datatype Type(float value) const { return MPI_FLOAT; }
    inline MPI_Datatype Type(double value) const { return MPI_DOUBLE; }
    inline MPI_Datatype Type(int v, int r) const { return MPI_2INT; }
    inline MPI_Datatype Type(long v, int r) const { return MPI_LONG_INT; }
    inline MPI_Datatype Type(float v, int r) const { return MPI_FLOAT_INT; }
    inline MPI_Datatype Type(double v, int r) const { return MPI_DOUBLE_INT; }
    inline void Barrier() const {
        MPI_Barrier(getCommunicator());
    }

    template<typename StatusType>
    void WaitAll(std::vector<StatusType>& statuses) const;

    template<typename T>
    void Recv(T& value, int source) const;

    template<typename T>
    void Recv(T& value) const;

    template<typename T>
    void Send(const T& value, int source) const;

    template<typename T>
    MessageStatus NonBlockingSend(const T& value, int destination) const;

    template<typename T>
    MessageStatus NonBlockingRecv(const T& value, int source) const;

    template<typename T>
    void Send(const std::vector<T>& vec, int length, int destination) const;

    template<typename T>
    MessageStatus NonBlockingSend(const std::vector<T>& vec, int length, int destination) const;

    template<typename T>
    MessageStatus NonBlockingSend(const std::vector<T>& vec, int destination) const;

    template<typename T>
    MessageStatus NonBlockingRecv(std::vector<T>& vec, int source) const;

    template<typename T>
    MessageStatus NonBlockingRecv(std::vector<T>& vec, int length, int source) const;

    template<typename T>
    MessageStatus NonBlockingRecv(T &d, int source) const;

    template<typename T>
    void Recv(std::vector<T>& vec, int length, int source) const;

    template<typename T>
    void Send(const std::vector<T>& vec, int destination) const;

    template<typename T>
    void Send(const std::vector<std::vector<T>>& vec, int destination) const;

    template<typename T>
    void Recv(std::vector<std::vector<T>>& vec, int source) const;

    template<typename T>
    void Recv(std::vector<T>& vec, int source) const;

    template<typename T>
    void Gather(T value, std::vector<T>& vec, int rootId) const;

    template<typename T>
    void AllGather(T value, std::vector<T>& vec) const;

    template<typename T>
    void AllGatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec, std::vector<int>& map) const;

    template<typename T>
    void AllGatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec) const;

    template<typename T>
    void AllGatherv(const std::vector<T>& send_vec, std::vector<std::vector<T>>& vec_of_vec_output) const;

    template<typename T>
    void Gather(const std::vector<T>& send_vec, int send_count, std::vector<T>& recv_vec, int rootId) const;

    template<typename T>
    void Gatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec,
                 std::vector<int>& map, int rootId) const;

    template<typename T>
    void Gatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec,
                 int rootId) const;

    template<typename T>
    void Gatherv(const std::vector<T>& send_vec,
                 std::vector<std::vector<T>>& result, int root_id) const;

    void Broadcast(std::string& s, int rootId) const;

    template<typename T>
    void Broadcast(T& value, int rootId) const;

    template<typename T>
    void Broadcast(std::vector<T>& vec, int vecLength, int rootId) const;

    template<typename T>
    void Broadcast(std::vector<T>& vec, int rootId) const;

    template<typename T>
    void Scatter(std::vector<T>& vec, T& recv_value, int rootId) const;

    template<typename T>
    void Scatter(std::vector<T>& vec, std::vector<T>& recv_vec, int rootId) const;

    template<typename T>
    void Scatterv(std::vector<T>& vec, std::vector<T>& recv_vec, int rootId) const;

    template<typename T>
    T ParallelSum(T value, int rootId) const;

    template<typename T>
    T ParallelSum(const T& value) const;

    template<typename T>
    std::vector<T> ParallelMax(const std::vector<T>& vec, int rootId) const;

    template<typename T>
    T ParallelMax(T value, int rootId) const;

    template<typename T>
    T ParallelMax(T value) const;

    template<typename T>
    std::vector<T> ParallelMin(const std::vector<T>& vec, int rootId) const;

    template<typename T>
    T ParallelMin(T value, int rootId) const;

    template<typename T>
    T ParallelMin(T value) const;

    template<typename T>
    T ParallelRankOfMax(T value, int rootId) const;

    template<typename T>
    std::vector<T> ElementalMax(std::vector<T>& vec, int root) const;

    template<typename T>
    std::vector<T> AllElementalMax(std::vector<T>& vec) const;
private:
    MPI_Comm communicator;
};

#include "MessagePasserSends.hpp"
#include "MessagePasserRecvs.hpp"
#include "MessagePasserWait.hpp"
#include "MessagePasserBroadcasts.hpp"
#include "MessagePasserScatters.hpp"
#include "MessagePasserReductions.hpp"
#include "MessagePasser.hpp"
#include "MessagePasserGathers.hpp"
