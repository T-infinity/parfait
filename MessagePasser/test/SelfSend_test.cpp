
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
#include <catch.hpp>
#include <vector>
#include "../MessagePasser.h"

TEST_CASE("Self Send"){
    auto mp = std::make_shared<MessagePasser>();

    std::vector<int> send(100,1);
    std::vector<int> recv;
    int tag = 0;

    if (mp->Rank() == 0) {
        MPI_Request request;
        MPI_Isend(send.data(), send.size(), MPI_INT, 0, tag, MPI_COMM_WORLD, &request);

        MPI_Status status;
        MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_INT, &count);
        if(count != 100)
            throw std::logic_error("Count incorrect size");
        recv.resize(count);
        MPI_Recv(recv.data(), count, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if(recv[0] != 1)
            throw std::logic_error("Recv incorrect value");
        MPI_Wait(&request, &status);
    }
}
