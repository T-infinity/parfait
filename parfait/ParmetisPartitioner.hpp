
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
#include <parfait/ParmetisPartitioner.h>
#include <MessagePasser/MessagePasser.h>
#include <parfait/ParmetisWrapper.h>

namespace Parfait {
    template<typename ContainerOfContainers>
    std::vector<int> ParmetisPartitioner::generatePartVector(
          MessagePasser mp,
          const ContainerOfContainers& node_to_node) {
      //convert connectivity to flat arrays
      std::vector<long> ia(node_to_node.size() + 1, 0);
      for (auto i = 0; i < node_to_node.size(); i++)
          ia[i + 1] = ia[i] + node_to_node[i].size();
      std::vector<long> ja;
      ja.reserve(ia.back());
      for (auto& row:node_to_node)
          for (auto nbr:row)
              ja.push_back(nbr);
      // map nodes to processors
      std::vector<long> procNodeMap(mp.NumberOfProcesses(), 0);
      mp.AllGather((long) node_to_node.size(), procNodeMap);
      procNodeMap.insert(procNodeMap.begin(), 0);
      for (unsigned int i = 1; i < procNodeMap.size(); i++)
          procNodeMap[i] += procNodeMap[i - 1];
      // allocate part vector and call 3rd party partitioner
      std::vector<int> part(node_to_node.size(), 0);
      partitionMesh(mp, mp.Rank(), mp.NumberOfProcesses(), procNodeMap.data(),
                    ia.data(), ja.data(), part.data());
      return part;
  }
}