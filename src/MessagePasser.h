#ifndef MESSAGE_PASSER_H
#define MESSAGE_PASSER_H
#include <vector>
#include <assert.h>
#include <stdio.h>
#ifdef PARFAIT_WITH_MPI

#include <mpi.h>
#include "MessagePasserStatus.h"

namespace MessagePasser
{
	void Init();
	void Finalize();
    void InitWithThreads();
	int Rank();
	int NumberOfProcesses();
	inline MPI_Datatype Type(int    value) {return MPI_INT;}
	inline MPI_Datatype Type(size_t value) {return MPI_INT;}
    inline MPI_Datatype Type(long   value) {return MPI_LONG;}
	inline MPI_Datatype Type(float  value) {return MPI_FLOAT;}
	inline MPI_Datatype Type(double value) {return MPI_DOUBLE;}
	inline void Barrier() {MPI_Barrier(MPI_COMM_WORLD);}

	template<typename StatusType>
    void Wait(StatusType& status);

    template<typename StatusType>
    void WaitAll(std::vector<StatusType>& statuses);

  	template <typename T>
	void Recv(T &value, int source);

  	template <typename T>
  	void Recv(T &value);

    template <typename T>
    void Send(T &value, int source);

	template<typename T>
	void Send(std::vector<T> &vec,int length,int destination);

    template<typename T>
    MessageStatus NonBlockingSend(std::vector<T>& vec,int length,int destination);

    template<typename T>
    MessageStatus NonBlockingSend(std::vector<T>& vec,int destination);

    template<typename T>
    MessageStatus NonBlockingRecv(std::vector<T>& vec,int length,int source);

	template<typename T>
	void Recv(std::vector<T> &vec,int length,int source);
	
	template<typename T>
	void Send(std::vector<T> &vec,int destination);

	template<typename T>
	void Send(std::vector<std::vector<T>> &vec,int destination);
	
	template<typename T>
	void Recv(std::vector<std::vector<T>> &vec,int source);

	template<typename T>
	void Recv(std::vector<T> &vec,int source);

	template<typename T>
	void Gather(T value,std::vector<T> &vec,int rootId);
	
	template<typename T>
	void AllGather(T value,std::vector<T> &vec);

	template<typename T>
	void AllGatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec,std::vector<int> &map);

	template<typename T>
	void AllGatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec);

	template <typename T>
	void AllGatherv(const std::vector<T>& send_vec, std::vector<std::vector<T>>& vec_of_vec_output);

	template<typename T>
	void Gather(const std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId);

	template<typename T>
	void Gather(const std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId);

	template<typename T>
	void Gatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec,
			std::vector<int> &map,int rootId);
	
    template<typename T>
	void Gatherv(const std::vector<T> &send_vec,std::vector<T> &recv_vec,
			int rootId);

	template<typename T>
	void Gatherv(const std::vector<T>& send_vec,
															std::vector<std::vector<T>>& result,int root_id);
	template<typename T>
	void Broadcast(T &value,int rootId);
	
	template<typename T>
	void Broadcast(std::vector<T> &vec,int vecLength,int rootId);

	template<typename T>
	void Broadcast(std::vector<T> &vec,int rootId);

	template<typename T>
	void Scatter(std::vector<T> &vec,T &recv_value,int rootId);

	template<typename T>
	void Scatter(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId);

	template<typename T>
	void Scatterv(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId);

	template<typename T>
	T ParallelSum(T value,int rootId);

    template <typename T>
    T ParallelSum(const T& value);

	template<typename T>
	T ParallelMax(T value,int rootId);

	template<typename T>
	std::vector<T> ParallelMax(const std::vector<T> &vec,int rootId);

	template<typename T>
	std::vector<T> ParallelMax(const std::vector<T> &vec,int rootId);

}
// -------- Implementation files -------------------------------------
#include "MessagePasserSends.hpp"
#include "MessagePasserRecvs.hpp"
#include "MessagePasserWait.hpp"
#include "MessagePasserBroadcasts.hpp"
#include "MessagePasserScatters.hpp"
#include "MessagePasserReductions.hpp"
#include "MessagePasser.hpp"
#include "MessagePasserGathers.hpp"

#else // define non-mpi dependent versions

#include "MessagePasserSerial.hpp"
#endif


#endif
