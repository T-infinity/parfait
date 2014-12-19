template<class MeshType>
Cell<MeshType>::Cell(MeshType &mesh_in,int cellId_in)
	: mesh(mesh_in),
	cellId(cellId_in)
{
}
template<class MeshType>
int Cell<MeshType>::numberOfNodes() const
{
	return mesh.numberOfNodesInCell(cellId);
}

template<class MeshType>
int Cell<MeshType>::numberOfFaces() const
{
	return mesh.numberOfFacesInCell(cellId);
}

template <class MeshType>
std::vector<int> Cell<MeshType>::getNodes() const
{ return mesh.getNodesInCell(Id());}

template<class MeshType>
void Cell<MeshType>::getNode(int i,double *p) const
{
	auto nodeIds = getNodes();
	mesh.getNode(nodeIds[i],p);
}

template<class MeshType>
Cell<MeshType>::Iterator::Iterator(Cell<MeshType> &cell_in,int index_in)
	: cell(cell_in),
	index(index_in)
{
}

template<class MeshType>
void Cell<MeshType>::Iterator::operator++()
{
	++index;
}

template<class MeshType>
bool Cell<MeshType>::Iterator::operator!=(const Iterator& rhs)
{
	if(rhs.index != index)
		return true;
	return false;
}

template<class MeshType>
CellFace<MeshType> Cell<MeshType>::Iterator::operator*()
{
	return CellFace<MeshType>(cell.mesh,cell.cellId,index);
}

template <class MeshType>
CellFace<MeshType> Cell<MeshType>::faces(int faceId)
{
    return CellFace<MeshType> (mesh, cellId, faceId);
}
