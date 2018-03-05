
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

template<typename T>
void MessagePasser::Send(const std::vector<T>& vec, int length, int destination) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::");
    MPI_Send(vec.data(), length * sizeof(T), MPI_CHAR, destination, 0, getCommunicator());
}

template<typename T>
void MessagePasser::Send(const std::vector<T>& vec, int destination) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::");
    int length = (int) vec.size();
    Send(vec, length, destination);
}

template<typename T>
void MessagePasser::Send(const std::vector<std::vector<T>>& vec, int destination) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::");
    // pack into a contiguous buffer
    std::vector<T> sendBuffer;
    std::vector<int> sendBufferMap = {0};
    for (auto& row:vec)
        sendBufferMap.push_back(row.size() + sendBufferMap.back());
    sendBuffer.reserve(sendBufferMap.back());
    for (auto& row:vec)
        for (auto val:row)
            sendBuffer.push_back(val);
    Send(sendBufferMap, destination);
    Send(sendBuffer, destination);
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingSend(const T& value, int destination) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::");
    MessageStatus status;
    MPI_Isend(&value, sizeof(T), MPI_CHAR, destination, 0, getCommunicator(), status.request());
    return status;
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingSend(
        const std::vector<T>& vec,
        int length, int destination) const {
    static_assert(std::is_trivially_copyable<T>::value, "Must be able to trivially copy datatype for MessagePasser::");
    MessageStatus status;
    MPI_Isend(vec.data(), length * sizeof(T), MPI_CHAR, destination, 0, getCommunicator(), status.request());
    return status;
}

template<typename T>
MessagePasser::MessageStatus MessagePasser::NonBlockingSend(
        const std::vector<T>& vec, int destination) const {
    return NonBlockingSend(vec, vec.size(), destination);
}
