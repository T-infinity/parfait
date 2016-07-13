#include "ParmetisPartitioner.h"
#include "MessagePasser.h"
#include "ParmetisWrapper.h"

namespace Parfait {
  inline std::vector<int> ParmetisPartitioner::generatePartVector(const std::vector<std::vector<long>> &node_to_node) {
      //convert connectivity to flat arrays
      for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
          if(MessagePasser::Rank() == r){
              printf("\nRank %d", r);
              printf("\nnode_to_node length %d", node_to_node.size());
              for(int row = 0; row < node_to_node.size(); row++){
                  printf("\nRow %d: ", row);
                  for(auto& n : node_to_node[row])
                      printf("%d, ", n);
              }
          }
          fflush(stdout);
          MessagePasser::Barrier();
      }
      std::vector<long> ia(node_to_node.size() + 1, 0);
      for (unsigned long i = 0; i < node_to_node.size(); i++)
          ia[i + 1] = ia[i] + node_to_node[i].size();
      std::vector<long> ja;
      ja.reserve(ia.back());
      for (auto &row:node_to_node)
          for (auto nbr:row)
              ja.push_back(nbr);
      // map nodes to processors
      std::vector<long> procNodeMap(MessagePasser::NumberOfProcesses(),0);
      MessagePasser::AllGather((long)node_to_node.size(), procNodeMap);
      procNodeMap.insert(procNodeMap.begin(), 0);
      for (unsigned int i = 1; i < procNodeMap.size(); i++)
          procNodeMap[i] += procNodeMap[i - 1];
      // allocate part vector and call 3rd party partitioner
      std::vector<int> part(node_to_node.size(), 0);
      partitionMesh(MessagePasser::Rank(), MessagePasser::NumberOfProcesses(), procNodeMap.data(),
                    ia.data(), ja.data(), part.data());
      return part;
  }
}