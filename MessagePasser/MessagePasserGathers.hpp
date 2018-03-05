
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
void MessagePasser::Gather(T value, std::vector<T>& vec, int rootId) const {
    vec.resize(NumberOfProcesses());
    MPI_Gather(&value, sizeof(T), MPI_CHAR, vec.data(), sizeof(T), MPI_CHAR, rootId, getCommunicator());
}

template<typename T>
void MessagePasser::AllGather(T value, std::vector<T>& vec) const {
    vec.resize(NumberOfProcesses());
    MPI_Allgather(&value, sizeof(T), MPI_CHAR, vec.data(), sizeof(T), MPI_CHAR, getCommunicator());
}

template<typename T>
void MessagePasser::Gather(const std::vector<T>& send_vec, int send_count, std::vector<T>& recv_vec, int rootId) const {
    if (Rank() == rootId) {
        recv_vec.clear();
        recv_vec.resize(send_count * NumberOfProcesses());
    }
    MPI_Gather(send_vec.data(), send_count * sizeof(T), MPI_CHAR, recv_vec.data(), send_count * sizeof(T),
               MPI_CHAR, rootId, getCommunicator());
}

template<typename T>
void MessagePasser::Gatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec,
                            std::vector<int>& map, int rootId) const {
    int sendcount = (int) send_vec.size() * sizeof(T);
    int nproc = NumberOfProcesses();
    std::vector<int> recv_counts(nproc, 0);
    Gather(sendcount, recv_counts, rootId);
    if (Rank() == rootId) {
        map.assign(nproc + 1, 0);
        for (int i = 1; i < nproc + 1; i++)
            map[i] = map[i - 1] + recv_counts[i - 1];
        recv_vec.resize(map.back() / sizeof(T));
    }
    MPI_Gatherv((void*) send_vec.data(), sendcount, MPI_CHAR,
                recv_vec.data(), recv_counts.data(), map.data(), MPI_CHAR,
                rootId, getCommunicator());
    // make map make sense for original datatype
    for (auto& x:map)
        x /= sizeof(T);
}

template<typename T>
void MessagePasser::Gatherv(const std::vector<T>& send_vec,
                            std::vector<std::vector<T>>& result, int root_id) const {
    std::vector<int> map;
    std::vector<T> recv;
    if (Rank() == root_id)
        result.assign(NumberOfProcesses(), std::vector<T>());
    Gatherv(send_vec, recv, map, root_id);
    if (Rank() == root_id) {
        for (int i = 0; i < NumberOfProcesses(); i++) {
            for (int j = map[i]; j < map[i + 1]; j++)
                result[i].push_back(recv[j]);
        }
    }
}

template<typename T>
void MessagePasser::Gatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec,
                            int rootId) const {
    std::vector<int> m;
    Gatherv(send_vec, recv_vec, m, rootId);
}

template<typename T>
void MessagePasser::AllGatherv(const std::vector<T>& send_vec, std::vector<T>& recv_vec) const {
    std::vector<int> m;
    AllGatherv(send_vec, recv_vec, m);
}

template<typename T>
void MessagePasser::AllGatherv(const std::vector<T>& send_vec,
                               std::vector<std::vector<T>>& vec_of_vec_output) const {
    std::vector<int> m;
    std::vector<T> recv_vec;
    AllGatherv(send_vec, recv_vec, m);
    for (unsigned int i = 0; i < m.size() - 1; i++) {
        int n = m[i + 1] - m[i];
        vec_of_vec_output.push_back(std::vector<T>(n, 0));
        for (int j = 0; j < n; j++) {
            vec_of_vec_output[i][j] = recv_vec[m[i] + j];
        }
    }
}

template<typename T>
void MessagePasser::AllGatherv(
        const std::vector<T>& send_vec,
        std::vector<T>& recv_vec, std::vector<int>& map) const {
    int sendcount = (int) send_vec.size() * sizeof(T);
    int nproc = NumberOfProcesses();
    std::vector<int> recv_counts(nproc, 0);
    AllGather(sendcount, recv_counts);
    map.assign(nproc + 1, 0);
    for (int i = 1; i < nproc + 1; i++)
        map[i] = map[i - 1] + recv_counts[i - 1];
    recv_vec.clear();
    recv_vec.resize(map.back() / sizeof(T));
    MPI_Allgatherv(&send_vec[0], sendcount, MPI_CHAR,
                   &recv_vec[0], &recv_counts[0], &map[0], MPI_CHAR,
                   getCommunicator());
    for (auto& x:map)
        x /= sizeof(T);
}
