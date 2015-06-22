#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>

template<class MeshType>
class ParMetisPrepper
{
	public:
		ParMetisPrepper(MeshType& mesh_in)  : mesh(mesh_in){}
		void buildNodeToNodeConnectivity();
		void buildCellToCellConnectivity();
		std::vector<int> getPartVector();
	private:
		ParMetisPrepper();
		MeshType& mesh;
		std::vector<int> procNodeMap;
		std::vector<std::vector<int> > connectivity;

};

#include "parmetis_prepper.hpp"

#endif
