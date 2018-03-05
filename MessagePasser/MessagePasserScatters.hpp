
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
void MessagePasser::Scatter(std::vector<T>& vec, T& recv_value, int rootId) const {
    MPI_Scatter(vec.data(), sizeof(T), MPI_CHAR, &recv_value, sizeof(T), MPI_CHAR, rootId, getCommunicator());
}

template<typename T>
void MessagePasser::Scatter(std::vector<T>& vec, std::vector<T>& recv_vec, int rootId) const {
    int size = 0;
    if (Rank() == rootId) {
        int total_length = (int) vec.size();
        assert(total_length % NumberOfProcesses() == 0); // must scatter equal amounts
        size = total_length / NumberOfProcesses();
    }
    Broadcast(size, rootId);
    recv_vec.resize(size);
    MPI_Scatter(vec.data(), size * sizeof(T), MPI_CHAR, recv_vec.data(), size * sizeof(T), MPI_CHAR, rootId,
                getCommunicator());
}

template<typename T>
void MessagePasser::Scatterv(std::vector<T>& vec, std::vector<T>& recv_vec, int rootId) const {
    std::vector<int> sendcounts;
    std::vector<int> displs;
    int local_size = 0;
    if (Rank() == rootId) {
        int total_size = (int) vec.size();
        int nproc = NumberOfProcesses();
        sendcounts.assign(nproc, total_size / nproc);
        for (int i = 0; i < nproc; i++)
            if (i < (total_size % nproc))
                sendcounts[i]++;
        displs.assign(nproc, 0);
        for (int i = 1; i < nproc; i++)
            displs[i] = displs[i - 1] + sendcounts[i - 1];
    }
    Scatter(sendcounts, local_size, rootId);
    recv_vec.resize(local_size);
    //convert sendcounts etc to char numbering
    for (auto& x:sendcounts)
        x *= sizeof(T);
    for (auto& x:displs)
        x *= sizeof(T);
    local_size *= sizeof(T);
    MPI_Scatterv(vec.data(), sendcounts.data(), displs.data(), MPI_CHAR,
                 recv_vec.data(), local_size, MPI_CHAR, rootId, getCommunicator());
}
