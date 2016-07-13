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
    //ComponentPlacer::movePoints(meshBuilder->metaData->xyz, meshBuilder->metaData->nodeComponentIds, motionMatrices);
    std::vector<BoundaryConditionMap> boundaryConditionMaps;
    for (int i = 0; i < config.numberOfGrids(); i++)
        boundaryConditionMaps.push_back(config.getMapbcObject(i));
    printf("file: %s\n",filename.c_str());
}

int parfait_get_number_of_nodes(){
    return applesauce.mesh->numberOfNodes();
}

void parfait_get_node_xyz(int n,double* p){
    auto point = applesauce.mesh->getXyz(n);
    p[0] = point[0];
    p[1] = point[1];
    p[2] = point[2];
}

int parfait_get_component_grid_id(int id){
    return applesauce.mesh->getNodeComponentId(id);
}

long parfait_get_global_node_id(int id){
    return applesauce.mesh->getGlobalNodeId(id);
}

int parfait_is_ghost_node(int id){
    return applesauce.mesh->isGhostNode(id);
}

int parfait_get_number_of_cells(){
    return applesauce.mesh->numberOfTets() +
           applesauce.mesh->numberOfPyramids() +
           applesauce.mesh->numberOfPrisms() +
           applesauce.mesh->numberOfHexes();
}

int parfait_get_number_of_nodes_in_cell(int n){
    return applesauce.getCellType(n);
}

void parfait_get_nodes_in_cell(int cell_id,int* cell){
    auto cellType = applesauce.getCellType(cell_id);
    if(cellType == TET) {
        auto c = applesauce.mesh->getTet(cell_id);
        for (int i = 0; i < 4; ++i)
            cell[i] = c[i];
        return;
    }
    cell_id -= applesauce.mesh->numberOfTets();

    if(cellType == PYRAMID){
        auto c = applesauce.mesh->getPyramid(cell_id);
        for (int i = 0; i < 5; ++i)
            cell[i] = c[i];
        return;
    }
    cell_id -= applesauce.mesh->numberOfPyramids();

    if(cellType == PRISM){
        auto c = applesauce.mesh->getPrism(cell_id);
        for (int i = 0; i < 6; ++i)
            cell[i] = c[i];
        return;
    }
    cell_id -= applesauce.mesh->numberOfPrisms();

    if(cellType == HEX){
        auto c = applesauce.mesh->getHex(cell_id);
        for (int i = 0; i < 8; ++i)
            cell[i] = c[i];
        return;
    }
}

int parfait_get_number_of_faces(){
    return applesauce.mesh->numberOfTriangles() + applesauce.mesh->numberOfQuads();
}

int parfait_get_number_of_nodes_in_face(int face_id){
    if(face_id < applesauce.mesh->numberOfTriangles())
        return 3;
    return 4;
}

int parfait_get_boundary_tag(int face_id){
    if(face_id < applesauce.mesh->numberOfTriangles())
        return applesauce.mesh->getTriangleTag(face_id);
    face_id -= applesauce.mesh->numberOfTriangles();
    return applesauce.mesh->getQuadTag(face_id);
}

void parfait_get_nodes_in_face(int face_id,int* nodes){
    if(face_id < applesauce.mesh->numberOfTriangles()) {
        auto c = applesauce.mesh->getTriangle(face_id);
        for (int i = 0; i < 3; ++i)
            nodes[i] = c[i];
    } else {
        auto c = applesauce.mesh->getQuad(face_id);
        for(int i=0;i<4;++i)
            nodes[i] = c[i];
    }
}

void parfait_finalize(){

}

CellType PersistentParfait::getCellType(int cell_id) {
    if(cell_id < mesh->numberOfTets())
        return TET;
    cell_id -= mesh->numberOfTets();
    if(cell_id < mesh->numberOfPyramids())
        return PYRAMID;
    cell_id -= mesh->numberOfPyramids();
    if(cell_id < mesh->numberOfPrisms())
        return PRISM;
    return HEX;
}
