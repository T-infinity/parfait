#ifndef GENERIC_SURFACE_MESH_H
#define GENERIC_SURFACE_MESH_H
namespace Parfait {
    template<class MeshType>
    class SurfaceMesh {
    protected:
        MeshType &mesh;
    public:
        SurfaceMesh(MeshType &mesh_in) : mesh(mesh_in) { }

        int numberOfBoundaryFaces() const { return mesh.numberOfBoundaryFaces(); }

        int numberOfNodesInBoundaryFace(int faceId) const {
            return mesh.numberOfNodesInBoundaryFace(faceId);
        }

        std::vector<int> getNodesInBoundaryFace(
                int faceId) const { return mesh.getNodesInBoundaryFace(faceId); }
    };
}

#endif
