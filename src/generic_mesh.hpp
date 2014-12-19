template<class MeshType>
Mesh<MeshType>::Mesh(MeshType &mesh_in)
	: mesh(mesh_in)
{
}

template <class MeshType>
Mesh<MeshType>::CellCollection::CellCollection(const Mesh<MeshType> &mesh_in) 
    : mesh(mesh_in)
{
}

template<class MeshType>
Mesh<MeshType>::CellCollection::Iterator::Iterator(const Mesh<MeshType> &mesh_in,int index_in)
	: mesh(mesh_in),
	index(index_in)
{
}

template<class MeshType>
void Mesh<MeshType>::CellCollection::Iterator::operator++()
{
	++index;
}

template<class MeshType>
bool Mesh<MeshType>::CellCollection::Iterator::operator!=(const Iterator& rhs)
{
	if(rhs.index != index)
		return true;
	return false;
}

template<class MeshType>
Cell<MeshType> Mesh<MeshType>::CellCollection::Iterator::operator*()
{
	return Cell<MeshType>(mesh.mesh,index);
}

//template <class MeshType>
//Cell<MeshType> Mesh<MeshType>::cells(int cellId) const
//{
//    return Cell<MeshType>(mesh, cellId);
//}
