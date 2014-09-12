template<class MeshType>
Node<MeshType>::Node(MeshType &mesh_in,int nodeId_in)
	: mesh(mesh_in),
	nodeId(nodeId_in)
{
}
#if 0
template<class MeshType>
int Face<MeshType>::numberOfNodes()
{
	return mesh->numberOfNodesInCellFace(cellId,faceId);
}

template<class MeshType>
void Face<MeshType>::getNodes(int *faceNodes)
{
	mesh->getNodesInCellFace(cellId,faceId,faceNodes);
}

template<class MeshType>
Face<MeshType>::Iterator::Iterator(Face<MeshType> *face_in,int index_in)
	: face(face_in),
	index(index_in)
{
}

template<class MeshType>
void Face<MeshType>::Iterator::operator++()
{
	++index;
}

template<class MeshType>
bool Face<MeshType>::Iterator::operator!=(const Iterator& rhs)
{
	if(rhs.index != index || rhs.face != face)
		return true;
	return false;
}

template<class MeshType>
int Face<MeshType>::Iterator::operator*()
{
	int vertices[index];
	face->getNodes(vertices);
	return vertices[index];
}
#endif
