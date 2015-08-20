//
// Created by cdruyorj on 8/20/15.
//

#include "parfait_preprocessor.h"
#include "PreProcessor.h"

class PersistentPreProcessorObject{
public:
    PersistentPreProcessorObject(const char*filename)
            :preprocessor(filename),
             mesh(preprocessor.createFun3DMesh()){}
    Parfait::PreProcessor preprocessor;
    Parfait::Fun3DMesh mesh;
};

int countCellsWithNodes(int m);

bool isQuad(const std::vector<int> &face);

bool isTriangle(const std::vector<int> &face);

PersistentPreProcessorObject* stuff = nullptr;

void throwIfNotInitialized(){
    if(nullptr == stuff)
        throw std::logic_error("Must initialize preprocessor before using it...");
}


void parfait_create_grid_system(const char *filename) {
    stuff = new PersistentPreProcessorObject(filename);
}

void parfait_cleanup() {
    if(nullptr != stuff) {
        stuff->mesh.freePointers();
        delete stuff;
    }
    stuff = nullptr;
}

int parfait_preprocessor_number_of_local_nodes() {
    throwIfNotInitialized();
    return stuff->mesh.numberOfNonGhostNodes();
}

int parfait_preprocessor_number_of_ghost_nodes() {
    throwIfNotInitialized();
    return stuff->mesh.numberOfNodes() - stuff->mesh.numberOfNonGhostNodes();
}

int parfait_preprocessor_number_of_triangles() {
    throwIfNotInitialized();
    int n = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++)
        if(3 == stuff->mesh.numberOfNodesInBoundaryFace(i))
            n++;
    return n;
}

int parfait_preprocessor_number_of_quads() {
    throwIfNotInitialized();
    int n = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++)
        if(4 == stuff->mesh.numberOfNodesInBoundaryFace(i))
            n++;
    return n;
}

int parfait_preprocessor_number_of_tets() {
    throwIfNotInitialized();
    return countCellsWithNodes(4);
}

int parfait_preprocessor_number_of_pyramids() {
    throwIfNotInitialized();
    return countCellsWithNodes(5);
}

int parfait_preprocessor_number_of_prisms() {
    throwIfNotInitialized();
    return countCellsWithNodes(6);
}

int parfait_preprocessor_number_of_hexs() {
    throwIfNotInitialized();
    return countCellsWithNodes(8);
}

int countCellsWithNodes(int m) {
    throwIfNotInitialized();
    int n = 0;
    for(int i=0;i<stuff->mesh.numberOfCells();i++)
        if(m == stuff->mesh.numberOfNodesInCell(i))
            n++;
    return n;
}

void parfait_preprocessor_fill_x(double *x) {
    throwIfNotInitialized();
    for(int i=0;i<stuff->mesh.numberOfNodes();i++)
        x[i] = stuff->mesh.getNode(i)[0];
}

void parfait_preprocessor_fill_y(double *y) {
    throwIfNotInitialized();
    for(int i=0;i<stuff->mesh.numberOfNodes();i++)
        y[i] = stuff->mesh.getNode(i)[0];
}

void parfait_preprocessor_fill_z(double *z) {
    throwIfNotInitialized();
    for(int i=0;i<stuff->mesh.numberOfNodes();i++)
        z[i] = stuff->mesh.getNode(i)[0];
}

void parfait_preprocessor_fill_global_node_ids(int *ids) {
    throwIfNotInitialized();
    for(int i=0;i<stuff->mesh.numberOfNodes();i++)
        ids[i] = stuff->mesh.getGlobalNodeId(i);
}

void parfait_preprocessor_fill_associated_grid_ids(int *ids) {
    throwIfNotInitialized();
    for(int i=0;i<stuff->mesh.numberOfNodes();i++)
        ids[i] = stuff->mesh.getImesh(i);
}

void parfait_preprocessor_fill_triangles(int *triangles) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++){
        auto face = stuff->mesh.getNodesInBoundaryFace(i);
        if(isTriangle(face))
            for(int id:face)
                triangles[iter++] = id;
    }
}

void parfait_preprocessor_fill_triangle_tags(int *tags) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++){
        auto face = stuff->mesh.getNodesInBoundaryFace(i);
        if(isTriangle(face))
            tags[iter++] = stuff->mesh.getBoundaryTag(i);
    }
}

void parfait_preprocessor_fill_triangle_bcs(int *bcs) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++){
        auto face = stuff->mesh.getNodesInBoundaryFace(i);
        if(isTriangle(face))
            bcs[iter++] = stuff->mesh.getBoundaryCondition(i);
    }
}

bool isTriangle(const std::vector<int> &face) { return 3 == face.size(); }

void parfait_preprocessor_fill_quads(int *quads) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++){
        auto face = stuff->mesh.getNodesInBoundaryFace(i);
        if(isQuad(face))
            for(int id:face)
                quads[iter++] = id;
    }
}

bool isQuad(const std::vector<int> &face) { return 4 == face.size(); }

void parfait_preprocessor_fill_quad_tags(int *quad_tags) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++){
        auto face = stuff->mesh.getNodesInBoundaryFace(i);
        if(isQuad(face))
            quad_tags[iter++] = stuff->mesh.getBoundaryTag(i);
    }
}

void parfait_preprocessor_fill_quad_bcs(int *quad_bcs) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfBoundaryFaces();i++){
        auto face = stuff->mesh.getNodesInBoundaryFace(i);
        if(isQuad(face))
            quad_bcs[iter++] = stuff->mesh.getBoundaryCondition(i);
    }
}

void parfait_preprocessor_fill_tets(int *tets) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfCells();i++){
        auto cell = stuff->mesh.getNodesInCell(i);
        if(4 == cell.size())
            for(int id:cell)
                tets[iter++] = id;
    }
}

void parfait_preprocessor_fill_pyramids(int *pyramids) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfCells();i++){
        auto cell = stuff->mesh.getNodesInCell(i);
        if(5 == cell.size())
            for(int id:cell)
                pyramids[iter++] = id;
    }
}

void parfait_preprocessor_fill_prisms(int *prisms) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfCells();i++){
        auto cell = stuff->mesh.getNodesInCell(i);
        if(6 == cell.size())
            for(int id:cell)
                prisms[iter++] = id;
    }
}

void parfait_preprocessor_fill_hexs(int *hexs) {
    throwIfNotInitialized();
    int iter = 0;
    for(int i=0;i<stuff->mesh.numberOfCells();i++){
        auto cell = stuff->mesh.getNodesInCell(i);
        if(8 == cell.size())
            for(int id:cell)
                hexs[iter++] = id;
    }
}
