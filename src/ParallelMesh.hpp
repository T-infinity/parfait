using namespace Parfait;

inline ParallelMeshBuilder::ParallelMeshBuilder() :
          metaData(std::make_shared<MeshParallelMetaData>()),
          connectivity(std::make_shared<MeshConnectivity>()){ }

inline ParallelMeshBuilder::ParallelMeshBuilder(ParallelMesh& mesh) :
    metaData(mesh.metaData), connectivity(mesh.connectivity){
}

inline std::shared_ptr<ParallelMesh> ParallelMeshBuilder::exportMesh() {
    return std::make_shared<Parfait::ParallelMesh>(connectivity, metaData);
}

inline int ParallelMesh::countNodesAtDegree(int degree) const {
    return int(std::count(metaData->nodeOwnershipDegree.begin(),
                      metaData->nodeOwnershipDegree.end(),degree));
}

inline int ParallelMesh::countNodesAboveDegree(int degree) const {
    int count = 0;
    for(auto& o : metaData->nodeOwnershipDegree)
        if(o > degree)
            count++;
    return count;
}

inline ParallelMesh::ParallelMesh()
        : connectivity(std::make_shared<MeshConnectivity>()),
          metaData(std::make_shared<MeshParallelMetaData>()){
}

inline int ParallelMesh::numberOfNodes() const {
    return metaData->xyz.size()/3;
}

inline Point<double> ParallelMesh::getXyz(int nodeId) const {
    return Parfait::Point<double>(&metaData->xyz[3*nodeId]);
}

inline int ParallelMesh::getTriangleTag(int id) const {
    return metaData->triangleTags[id];
}

inline int ParallelMesh::getQuadTag(int id) const {
    return metaData->quadTags[id];
}

inline int ParallelMesh::getNodeComponentId(int id) const {
    return metaData->nodeComponentIds[id];
}

inline long ParallelMesh::getGlobalNodeId(int id) const {
    return metaData->globalNodeIds[id];
}

inline int ParallelMesh::isGhostNode(int id) const {
    return metaData->nodeOwnershipDegree[id] != 0;
}

inline ParallelMesh::ParallelMesh(std::shared_ptr<MeshConnectivity> conn, std::shared_ptr<MeshParallelMetaData> mData) {
    metaData = mData;
    connectivity = conn;
}

inline int ParallelMesh::numberOfTets() const {
    return int(connectivity->tets.size() / 4);
}

inline const int* ParallelMesh::getTet(int id) const {
    return &(connectivity->tets[4*id]);
}

inline int ParallelMesh::numberOfPyramids() const {
    return int(connectivity->pyramids.size() / 5);
}

inline const int *ParallelMesh::getPyramid(int id) const {
    return &(connectivity->pyramids[5*id]);
}

inline int ParallelMesh::numberOfPrisms() const {
    return int(connectivity->prisms.size() / 6);
}

inline const int* ParallelMesh::getPrism(int id) const {
    return &(connectivity->prisms[6*id]);
}

inline int ParallelMesh::numberOfHexes() const {
    return int(connectivity->hexs.size() / 8);
}

inline const int *ParallelMesh::getHex(int id) const {
    return &(connectivity->hexs[8*id]);
}
inline int ParallelMesh::numberOfTriangles() const {
    return int(connectivity->triangles.size() / 3);
}
inline const int* ParallelMesh::getTriangle(int id) const {
    return &(connectivity->triangles[3*id]);
}
inline int ParallelMesh::numberOfQuads() const {
    return int(connectivity->quads.size() / 4);
}
inline const int* ParallelMesh::getQuad(int id) const {
    return &(connectivity->quads[4*id]);
}