template<class MeshType>
CellFace<MeshType>::CellFace(MeshType &mesh_in,int cellId_in,int faceId_in)
	: mesh(mesh_in),
	cellId(cellId_in),
	faceId(faceId_in)
{
}

template<class MeshType>
int CellFace<MeshType>::numberOfNodes() const
{
	return mesh.numberOfNodesInCellFace(cellId,faceId);
}

template<class MeshType>
std::vector<int> CellFace<MeshType>::getNodes() const
{
	return mesh.getNodesInCellFace(cellId,faceId);
}

template<class MeshType>
void CellFace<MeshType>::getNode(int i,double *p) const
{
	auto nodeIds = getNodes();
	mesh.getNode(nodeIds[i],p);
}

template<class MeshType>
CellFace<MeshType>::Iterator::Iterator(CellFace<MeshType> &face_in,int index_in)
	: face(face_in),
	index(index_in)
{
}

template<class MeshType>
void CellFace<MeshType>::Iterator::operator++()
{
	++index;
}

template<class MeshType>
bool CellFace<MeshType>::Iterator::operator!=(const Iterator& rhs)
{
	if(rhs.index != index)
		return true;
	return false;
}

template<class MeshType>
int CellFace<MeshType>::Iterator::operator*()
{
    auto vertices = face.getNodes();
	return vertices[index];
}
