#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>

template<class MeshType>
class ParMetisPrepper
{
	template<class T> using vector = std::vector<T>;
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

#include "ParmetisPrepper.hpp"

#endif
