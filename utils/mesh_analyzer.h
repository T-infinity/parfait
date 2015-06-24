#ifndef MESH_ANALYZER_H
#define MESH_ANALYZER_H
#include <generic_mesh.h>
#include "generic_surface_mesh.h"
#include <vector>

template<typename MeshType>
class MeshAnalyzer{
    public: 
        MeshAnalyzer(MeshType &m):mesh(m),surface(m){}
        void printStats();
        void throwIfBadIndexFound();
    private:
        Parfait::Mesh<MeshType> mesh;
        Parfait::SurfaceMesh<MeshType> surface;
};

template<typename MeshType>
void MeshAnalyzer<MeshType>::printStats(){
    printf("Number of nodes:             %i\n",mesh.numberOfNodes());
    printf("Number of cells:             %i\n",mesh.numberOfCells());
    printf("Number of boundary faces:    %i\n",surface.numberOfBoundaryFaces());
}

template<typename MeshType>
void MeshAnalyzer<MeshType>::throwIfBadIndexFound(){
    for(auto cell:mesh.cells()){
        for(int id:cell.getNodes()){
            if(id > mesh.numberOfNodes()){
                throw std::logic_error("Bad node id");
            }
        }
    }

}


#endif
