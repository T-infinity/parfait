using namespace Parfait;

inline ParallelMeshBuilder::ParallelMeshBuilder() :
          data(std::make_shared<ParallelMeshData>()){ }

inline ParallelMeshBuilder::ParallelMeshBuilder(ParallelMesh mesh) :
    data(mesh.data){
}

inline std::shared_ptr<ParallelMesh> ParallelMeshBuilder::exportMesh() {
    return std::make_shared<Parfait::ParallelMesh>(*this);
}

inline int ParallelMesh::countNodesAtDegree(int degree) const {
    return int(std::count(data->nodeOwnershipDegree.begin(),
                      data->nodeOwnershipDegree.end(),degree));
}

inline ParallelMesh::ParallelMesh(ParallelMeshBuilder builder)
        : data(builder.data){
}

inline int ParallelMesh::countNodesAboveDegree(int degree) const {
    int count = 0;
    for(auto& o : data->nodeOwnershipDegree)
        if(o > degree)
            count++;
    return count;
}

inline int ParallelMesh::numberOfNodes() const {
    return data->xyz.size()/3;
}

inline Point<double> ParallelMesh::getXyz(int nodeId) const {
    return Parfait::Point<double>(&data->xyz[3*nodeId]);
}

inline int ParallelMesh::getTriangleTag(int id) const {
    return data->triangleTags[id];
}

inline int ParallelMesh::getQuadTag(int id) const {
    return data->quadTags[id];
}

inline int ParallelMesh::getNodeComponentId(int id) const {
    return data->nodeComponentIds[id];
}

inline long ParallelMesh::getGlobalNodeId(int id) const {
    return data->globalNodeIds[id];
}

inline int ParallelMesh::isGhostNode(int id) const {
    return data->nodeOwnershipDegree[id] != 0;
}

inline int ParallelMesh::numberOfTets() const {
    return int(data->tets.size() / 4);
}

inline const int* ParallelMesh::getTet(int id) const {
    return &(data->tets[4*id]);
}

inline int ParallelMesh::numberOfPyramids() const {
    return int(data->pyramids.size() / 5);
}

inline const int *ParallelMesh::getPyramid(int id) const {
    return &(data->pyramids[5*id]);
}

inline int ParallelMesh::numberOfPrisms() const {
    return int(data->prisms.size() / 6);
}

inline const int* ParallelMesh::getPrism(int id) const {
    return &(data->prisms[6*id]);
}

inline int ParallelMesh::numberOfHexes() const {
    return int(data->hexs.size() / 8);
}

inline const int *ParallelMesh::getHex(int id) const {
    return &(data->hexs[8*id]);
}
inline int ParallelMesh::numberOfTriangles() const {
    return int(data->triangles.size() / 3);
}
inline const int* ParallelMesh::getTriangle(int id) const {
    return &(data->triangles[3*id]);
}
inline int ParallelMesh::numberOfQuads() const {
    return int(data->quads.size() / 4);
}
inline const int* ParallelMesh::getQuad(int id) const {
    return &(data->quads[4*id]);
}

inline int ParallelMesh::numberOfCells() const {
    return numberOfTets() + numberOfPyramids() + numberOfPrisms() + numberOfHexes();
}

inline int ParallelMesh::numberOfNodesInCell(int id) const {
    if(id < numberOfTets())
        return 4;
    id -= numberOfTets();
    if(id < numberOfPyramids())
        return 5;
    id -= numberOfPyramids();
    if(id < numberOfPrisms())
        return 6;
    return 8;
}

inline const int *ParallelMesh::getCell(int id) const {
    if(id < numberOfTets())
        return getTet(id);
    id -= numberOfTets();
    if(id < numberOfPyramids())
        return getPyramid(id);
    id -= numberOfPyramids();
    if(id < numberOfPrisms())
        return getPrism(id);
    id -= numberOfPrisms();
    return getHex(id);

}

inline int ParallelMesh::numberOfFaces() const {
    return numberOfTriangles() + numberOfQuads();
}

inline const int *ParallelMesh::getFace(int id) const {
    if(id < numberOfTriangles())
        return getTriangle(id);
    id -= numberOfTriangles();
    return getQuad(id);
}

inline int ParallelMesh::numberOfNodesInFace(int id) const {
    if(id < numberOfTriangles())
        return 3;
    return 4;
}

inline int ParallelMesh::getFaceTag(int id) const {
    if(id < numberOfTriangles())
        return getTriangleTag(id);
    id -= numberOfTriangles();
    return getQuadTag(id);
}














