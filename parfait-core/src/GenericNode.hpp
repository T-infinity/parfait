namespace Parfait {
    template<class MeshType>
    Node<MeshType>::Node(MeshType &mesh_in, int nodeId_in)
            : mesh(mesh_in),
              nodeId(nodeId_in) {
    }
}
