#pragma once

#include "GenericMesh.h"
#include "GenericMeshTools.h"

namespace GenericMeshAcceptanceChecks {

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