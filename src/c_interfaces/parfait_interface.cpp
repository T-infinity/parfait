#include "parfait_interface.h"
#include <string>
#include <array>
#include <ConfigurationReader.h>
#include <PreProcessor.h>

using namespace Parfait;
using namespace std;

enum CellType {TET = 4, PYRAMID = 5, PRISM = 6, HEX = 8};

class PersistentParfait{
  public:
    PersistentParfait() = default;
    shared_ptr<ParallelMesh> mesh;
    CellType getCellType(int cell_id);
    int num_tets;
    int num_pyramids;
    int num_prisms;
    int num_hexs;
    int num_triangles;
    int num_quads;
};

static PersistentParfait applesauce;

void parfait_initialize(const char* fname){
  std::string filename(fname);
  ConfigurationReader configurationReader(fname);
  auto config = configurationReader.createConfiguration();
  PreProcessor preProcessor(config);
  applesauce.mesh = preProcessor.createMesh();
  std::vector<MotionMatrix> motionMatrices;
  for (int i = 0; i < config.numberOfGrids(); ++i)
    motionMatrices.push_back(config.getMotionMatrix(i));
  //ComponentPlacer::movePoints(mesh->metaData->xyz, mesh->metaData->nodeComponentIds, motionMatrices);
  std::vector<BoundaryConditionMap> boundaryConditionMaps;
  for (int i = 0; i < config.numberOfGrids(); i++)
    boundaryConditionMaps.push_back(config.getMapbcObject(i));
  printf("file: %s\n",filename.c_str());

  applesauce.num_tets = applesauce.mesh->connectivity->tets.size() / 4;
  applesauce.num_pyramids = applesauce.mesh->connectivity->pyramids.size() / 5;
  applesauce.num_prisms = applesauce.mesh->connectivity->prisms.size() / 6;
  applesauce.num_hexs = applesauce.mesh->connectivity->hexs.size() / 8;
  applesauce.num_triangles = applesauce.mesh->connectivity->triangles.size() / 3;
  applesauce.num_quads = applesauce.mesh->connectivity->quads.size() / 4;
}

int parfait_get_number_of_nodes(){
  return applesauce.mesh->metaData->xyz.size()/3;
}

void parfait_get_node_xyz(int n,double* p){
  p[0] = applesauce.mesh->metaData->xyz[3*n+0];
  p[1] = applesauce.mesh->metaData->xyz[3*n+1];
  p[2] = applesauce.mesh->metaData->xyz[3*n+2];
}

int parfait_get_component_grid_id(int id){
  return applesauce.mesh->metaData->nodeComponentIds[id];
}

long parfait_get_global_node_id(int id){
  return applesauce.mesh->metaData->globalNodeIds[id];
}

int parfait_get_node_ownership(int id){
  return applesauce.mesh->metaData->nodeOwnershipDegree[id];
}

int parfait_get_number_of_cells(){
  return applesauce.mesh->connectivity->tets.size()/4
          +applesauce.mesh->connectivity->pyramids.size()/5
          +applesauce.mesh->connectivity->prisms.size()/6
          + applesauce.mesh->connectivity->hexs.size() / 8;
}

int parfait_get_number_of_nodes_in_cell(int n){
  return applesauce.getCellType(n);
}

void parfait_get_nodes_in_cell(int n,int* cell){
  auto cellType = applesauce.getCellType(n);
  if(cellType == TET) {
    for (int i = 0; i < 4; ++i)
      cell[i] = applesauce.mesh->connectivity->tets[4 * n + i];
    return;
  }
  if(cellType == PYRAMID){
    for (int i = 0; i < 5; ++i)
      cell[i] = applesauce.mesh->connectivity->pyramids[5 * n + i];
    return;
  }
  if(cellType == PRISM){
    for (int i = 0; i < 6; ++i)
      cell[i] = applesauce.mesh->connectivity->prisms[6 * n + i];
    return;
  }
  if(cellType == HEX){
    for (int i = 0; i < 8; ++i)
      cell[i] = applesauce.mesh->connectivity->hexs[8 * n + i];
    return;
  }
}

int parfait_get_number_of_faces(){
  return applesauce.num_triangles + applesauce.num_quads;
}

int parfait_get_number_of_nodes_in_face(int face_id){
  if(face_id < applesauce.num_triangles)
    return 3;
  return 4;
}

int parfait_get_boundary_tag(int face_id){
  if(face_id < applesauce.num_triangles)
    return applesauce.mesh->metaData->triangleTags[face_id];
  face_id -= applesauce.num_triangles;
  return applesauce.mesh->metaData->quadTags[face_id];
}

void parfait_get_nodes_in_face(int face_id,int* nodes){
  if(face_id < applesauce.num_triangles)
    for(int i=0;i<3;++i)
      nodes[i] = applesauce.mesh->connectivity->triangles[3*face_id+i];
  else
    for(int i=0;i<4;++i)
      nodes[i] = applesauce.mesh->connectivity->quads[4*face_id+i];
}

void parfait_finalize(){

}

CellType PersistentParfait::getCellType(int cell_id) {
  if(cell_id < num_tets)
    return TET;
  cell_id -= num_tets;
  if(cell_id < num_pyramids)
    return PYRAMID;
  cell_id -= num_pyramids;
  if(cell_id < num_prisms)
    return PRISM;
  return HEX;
}
