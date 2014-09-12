#ifndef GENERIC_NODE_H
#define GENERIC_NODE_H

template<class MeshType>
class Node
{
	public:
		Node(MeshType &mesh,int nodeId);
        int Id(){return nodeId;}
#if 0
		int numberOfNodes();
		void getNodes(int *faceNodes);
		class Iterator
		{
			public:
				Iterator(Face<MeshType> *face,int index);
				void operator++();
				bool operator!=(const Iterator& rhs);
				int  operator*();
			private:
				Face<MeshType> *face;
				int index;
		};
		Iterator begin() {return Iterator(this,0);}
		Iterator end()   {return Iterator(this,mesh->numberOfNodesInCellFace(cellId,faceId));}
#endif
	private:
		MeshType &mesh;
		int nodeId;
};

#include "generic_node.hpp"

#endif
