#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>

using std::vector;

template<class MeshType>
class ParMetisPrepper
{
	public:
		ParMetisPrepper(MeshType& mesh_in)  : mesh(mesh_in){}
		void buildNodeToNodeConnectivity();
		void buildCellToCellConnectivity();
		vector<int> getPartVector();
	private:
		ParMetisPrepper();
		MeshType& mesh;
		vector<int> procNodeMap;
		vector<vector<int> > connectivity;

};

#include "parmetis_prepper.hpp"

#endif
