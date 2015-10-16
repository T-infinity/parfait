#include <MessagePasser.h>
#include <ParmetisWrapper.h>
#include <NodeToNode.h>

namespace Parfait {
  inline std::vector<int> Partitioner::generatePartVector(const std::vector<std::vector<long>> &node_to_node) {
	  //convert connectivity to flat arrays
	  std::vector<long> ia(node_to_node.size() + 1, 0);
	  for (long i = 0; i < node_to_node.size(); i++)
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
	  for (int i = 1; i < procNodeMap.size(); i++)
		  procNodeMap[i] += procNodeMap[i - 1];
	  // allocate part vector and call 3rd party partitioner
	  std::vector<int> part(node_to_node.size(), 0);
	  PartitionMesh(MessagePasser::Rank(), MessagePasser::NumberOfProcesses(), procNodeMap.data(),
					ia.data(), ja.data(), part.data());
	  return part;
  }
}