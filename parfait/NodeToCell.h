#include <vector>
#include <algorithm>

namespace Parfait{

	template<typename MeshType>
	auto buildNodeToCell(MeshType& mesh){
		std::vector<std::vector<int>> n2c(mesh.numberOfNodes());
		for(int cellId = 0; cellId < mesh.numberOfCells(); cellId++){
			auto cell = mesh.getNodesInCell(cellId);
			for(auto node:cell){
				auto iter = std::find(n2c[node].begin(), n2c[node].end(), cellId);
				if(iter == n2c[node].end())
					n2c[node].push_back(cellId);
			}
		}
		return n2c;
	}

}
