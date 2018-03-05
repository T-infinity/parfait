
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include <stdexcept>
#include <mpi.h>

inline MessagePasser::MessagePasser()
        :communicator(MPI_COMM_WORLD) {
}
inline MessagePasser::MessagePasser(MPI_Comm comm)
        : communicator(comm) {

}
inline MPI_Comm MessagePasser::getCommunicator() const {
    return communicator;
}

inline void MessagePasser::setCommunicator(MPI_Comm comm) {
    communicator = comm;
}


inline void MessagePasser::Init() {
    int initialized = 0;
    MPI_Initialized(&initialized);
    if (not initialized)
        MPI_Init(NULL, NULL);
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
