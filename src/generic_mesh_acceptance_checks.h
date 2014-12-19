#ifndef GENERIC_MESH_ACCEPTANCE_CHECKS_H
#define GENERIC_MESH_ACCEPTANCE_CHECKS_H

#include "generic_mesh.h"
#include "generic_mesh_tools.h"

namespace GenericMeshAcceptanceChecks
{

    template <typename MeshType> 
    bool isMeshFacesOutward(MeshType &mesh_in){
        Mesh<MeshType> mesh(mesh_in);

        for(auto cell : mesh.cells()){
            Point center = GenericMeshTools::cellCenter(mesh_in, cell);
            for(auto face :cell){
                Point faceCenter = GenericMeshTools::faceCenter(mesh_in, face);
                Point faceArea = GenericMeshTools::getFaceArea(mesh_in, face);

                Point cellOutward = faceCenter - center;
                if(Point::dot(faceArea, cellOutward) <= 0){
                   return false; 
                }
            }
        }
        return true;
    }
};

#endif
