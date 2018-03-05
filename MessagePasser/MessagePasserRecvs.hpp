
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
#include <type_traits>

template<typename T>
void MessagePasser::Recv(std::vector<T>& vec, int length, int source) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    vec.clear();
    vec.resize(length);
    MPI_Status status;
    MPI_Recv(vec.data(), length * sizeof(T), MPI_CHAR, source, 0, getCommunicator(), &status);
}

template<typename T>
void MessagePasser::Recv(std::vector<T>& vec, int source) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    int n = 0;
    MPI_Status status;
    MPI_Probe(source, 0, getCommunicator(), &status);
    MPI_Get_count(&status, MPI_CHAR, &n);
    vec.resize(n / sizeof(T));
    MPI_Recv(vec.data(), n, MPI_CHAR, source, 0, getCommunicator(), MPI_STATUS_IGNORE);
}

template<typename T>
void MessagePasser::Recv(std::vector<std::vector<T>>& vec, int source) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    std::vector<int> recvBufferMap;
    std::vector<T> recvBuffer;
    Recv(recvBufferMap, source);
    Recv(recvBuffer, source);
    vec.assign(recvBufferMap.size() - 1, std::vector<T>());
    // use map to unpack buffer into vector of vectors
    for (unsigned int i = 0; i < recvBufferMap.size() - 1; i++)
        for (int j = recvBufferMap[i]; j < recvBufferMap[i + 1]; j++)
            vec[i].push_back(recvBuffer[j]);
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingRecv(
        std::vector<T>& vec, int length, int source) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    MessageStatus status;
    vec.resize(length);
    MPI_Irecv(vec.data(), length * sizeof(T), MPI_CHAR, source, 0, getCommunicator(), status.request());
    return status;
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingRecv(
    std::vector<T>& vec, int source) const {
    static_assert(sizeof(T) != sizeof(T), "NonBlockingRecv does not work for vectors of unknown size");
    throw std::logic_error("Can not MessagePasser::NonBlockingRecv with vector of unknown size");
}


template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingRecv(T &d, int source) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::NonBlockingRecv");
    MessageStatus status;
    MPI_Irecv(&d, 1 * sizeof(T), MPI_CHAR, source, 0, getCommunicator(), status.request());
    return status;
}

