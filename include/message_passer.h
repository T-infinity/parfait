#ifndef MESSAGE_PASSER_H
#define MESSAGE_PASSER_H
#include <vector>
#include <assert.h>
#ifdef PARFAIT_WITH_MPI

#include <mpi.h>
namespace MessagePasser
{
	// Basic utility functions
	void Init();
	void Finalize();
	int Rank();
	int NumberOfProcesses();
	// Overloaded functions to return correct MPI Datatype without dynamic type checking
	inline MPI_Datatype Type(int    value) {return MPI_INT;}
	inline MPI_Datatype Type(float  value) {return MPI_FLOAT;}
	inline MPI_Datatype Type(double value) {return MPI_DOUBLE;}
	inline void Barrier() {MPI_Barrier(MPI_COMM_WORLD);}
	
	//-------------- point to point communication ---------------------
	//Send a single value
	template<typename T>
	void Send(T value,int destination);
	
	// recv a single value
	template<typename T>	
	void Recv(T &value,int source);

	// recv a single value from any source
	template<typename T>	
	void Recv(T &value);

	// send a vector (receiver knows length)
	template<typename T>
	void Send(std::vector<T> &vec,int length,int destination);

	// recv a vector (receiver knows length)
	template<typename T>
	void Recv(std::vector<T> &vec,int length,int source);
	
	// send a vector (receiver doesn't know length)
	template<typename T>
	void Send(std::vector<T> &vec,int destination);

	// send a vector of vectors
	template<typename T>
	void Send(std::vector<std::vector<T>> &vec,int destination);
	
	// recv a vector of vectors
	template<typename T>
	void Recv(std::vector<std::vector<T>> &vec,int source);

	// recv a vector (receiver doesn't know length)
	template<typename T>
	void Recv(std::vector<T> &vec,int source);

	// -------------  collective communication -----------------------
	// Gather values to a vector on the root
	template<typename T>
	void Gather(T value,std::vector<T> &vec,int rootId);
	
	// Gather values to a vector on every proc
	template<typename T>
	void AllGather(T value,std::vector<T> &vec);

	// Gather distributed vectors to a single vector on every proc
	template<typename T>
	void AllGatherv(std::vector<T> &send_vec,std::vector<T> &recv_vec,std::vector<int> &map);

	// Gather vectors of a given size to the root
	template<typename T>
	void Gather(std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId);
	// Gather vectors of a given size to the root
	template<typename T>
	void Gather(std::vector<T> &send_vec,int send_count,std::vector<T> &recv_vec,int rootId);

	// Gatherv vectors of different lengths to the root
	template<typename T>
	void Gatherv(std::vector<T> &send_vec,std::vector<T> &recv_vec,
			std::vector<int> &map,int rootId);

	// broadcast a value
	template<typename T>
	void Broadcast(T &value,int rootId);
	
	// broadcast a vector of KNOWN size
	template<typename T>
	void Broadcast(std::vector<T> &vec,int vecLength,int rootId);

	// broadcast a vector of unknown size
	template<typename T>
	void Broadcast(std::vector<T> &vec,int rootId);

	// scatter a vector that has exactly nproc 	elements
	template<typename T>
	void Scatter(std::vector<T> &vec,T &recv_value,int rootId);

	// scatter a vector that divides evenly into the number of procs
	template<typename T>
	void Scatter(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId);

	// scatterv a vector that does not divide evenly into number of procs
	template<typename T>
	void Scatterv(std::vector<T> &vec,std::vector<T> &recv_vec,int rootId);

	// ----------- Reduction operations ----------------------------------
	// sum single values to the root
	template<typename T>
	T ParallelSum(T value,int rootId);
	
	// get the max of values in a vector
	template<typename T>
	std::vector<T> ParallelMax(const std::vector<T> &vec,int rootId);

	// -------- Implementation files -------------------------------------
	#include "message_passer_sends.hpp"
	#include "message_passer_recvs.hpp"
	#include "message_passer_gathers.hpp"
	#include "message_passer_broadcasts.hpp"
	#include "message_passer_scatters.hpp"
	#include "message_passer_reductions.hpp"	
}

#else // define non-mpi dependent versions

#include "message_passer_serial.hpp"
#endif


#endif
