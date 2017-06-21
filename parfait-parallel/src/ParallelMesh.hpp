using namespace Parfait;
#include <MessagePasser.h>

inline ParallelMeshBuilder::ParallelMeshBuilder(std::shared_ptr<MessagePasser> mp) :
        messagePasser(mp), data(std::make_shared<ParallelMeshData>()) {}

inline ParallelMeshBuilder::ParallelMeshBuilder(std::shared_ptr<MessagePasser> mp,
                                                ParallelMesh mesh) :
        messagePasser(mp), data(mesh.data) {
}

inline std::shared_ptr<ParallelMesh> ParallelMeshBuilder::exportMesh() {
    return std::make_shared<Parfait::ParallelMesh>(messagePasser, *this);
}

inline int ParallelMesh::countOwnedNodes() const {
    int rank = messagePasser->Rank();
    return int(std::count(data->nodeOwnerRank.begin(),
                          data->nodeOwnerRank.end(), rank));
}

inline ParallelMesh::ParallelMesh(std::shared_ptr<MessagePasser> messagePasser,
                                  ParallelMeshBuilder builder)
        : messagePasser(messagePasser), data(builder.data) {
}

inline int ParallelMesh::numberOfNodes() const {
    return data->xyz.size() / 3;
}

inline Point<double> ParallelMesh::getXyz(int nodeId) const {
    return Parfait::Point<double>(&data->xyz[3 * nodeId]);
}

inline int ParallelMesh::getTriangleTag(int id) const {
    return data->triangleTags[id];
}

inline int ParallelMesh::getQuadTag(int id) const {
    return data->quadTags[id];
}

inline int ParallelMesh::owningRankOfNode(int id) const {
    return data->nodeOwnerRank[id];
}

inline int ParallelMesh::getNodeComponentId(int id) const {
    return data->nodeComponentIds[id];
}

inline long ParallelMesh::getGlobalNodeId(int id) const {
    return data->globalNodeIds[id];
}

inline int ParallelMesh::isGhostNode(int id) const {
    return data->nodeOwnerRank[id] != messagePasser->Rank();
}

inline int ParallelMesh::numberOfTets() const {
    return int(data->tets.size() / 4);
}

inline const int* ParallelMesh::getTet(int id) const {
    return &(data->tets[4 * id]);
}

inline int ParallelMesh::numberOfPyramids() const {
    return int(data->pyramids.size() / 5);
}

inline const int* ParallelMesh::getPyramid(int id) const {
    return &(data->pyramids[5 * id]);
}

inline int ParallelMesh::numberOfPrisms() const {
    return int(data->prisms.size() / 6);
}

inline const int* ParallelMesh::getPrism(int id) const {
    return &(data->prisms[6 * id]);
}

inline int ParallelMesh::numberOfHexes() const {
    return int(data->hexs.size() / 8);
}

inline const int* ParallelMesh::getHex(int id) const {
    return &(data->hexs[8 * id]);
}
inline int ParallelMesh::numberOfTriangles() const {
    return int(data->triangles.size() / 3);
}
inline const int* ParallelMesh::getTriangle(int id) const {
    return &(data->triangles[3 * id]);
}
inline int ParallelMesh::numberOfQuads() const {
    return int(data->quads.size() / 4);
}
inline const int* ParallelMesh::getQuad(int id) const {
    return &(data->quads[4 * id]);
}

inline int ParallelMesh::numberOfCells() const {
    return numberOfTets() + numberOfPyramids() + numberOfPrisms() + numberOfHexes();
}

inline int ParallelMesh::numberOfNodesInCell(int id) const {
    if (id < numberOfTets())
        return 4;
    id -= numberOfTets();
    if (id < numberOfPyramids())
        return 5;
    id -= numberOfPyramids();
    if (id < numberOfPrisms())
        return 6;
    return 8;
}

inline const int* ParallelMesh::getCell(int id) const {
    if (id < numberOfTets())
        return getTet(id);
    id -= numberOfTets();
    if (id < numberOfPyramids())
        return getPyramid(id);
    id -= numberOfPyramids();
    if (id < numberOfPrisms())
        return getPrism(id);
    id -= numberOfPrisms();
    return getHex(id);
}

inline std::vector<int> ParallelMesh::getNodesInCell(int id) const {
    auto cell_ptr = getCell(id);
    std::vector<int> cell;
    for (int i = 0; i < numberOfNodesInCell(id); ++i)
        cell.push_back(cell_ptr[i]);
    return cell;
}

inline std::vector<int> ParallelMesh::getNodesInFace(int id) const {
    auto face_ptr = getFace(id);
    std::vector<int> face;
    for (int i = 0; i < numberOfNodesInFace(id); ++i)
        face.push_back(face_ptr[i]);
    return face;
}

inline int ParallelMesh::numberOfFaces() const {
    return numberOfTriangles() + numberOfQuads();
}

inline const int* ParallelMesh::getFace(int id) const {
    if (id < numberOfTriangles())
        return getTriangle(id);
    id -= numberOfTriangles();
    return getQuad(id);
}

inline int ParallelMesh::numberOfNodesInFace(int id) const {
    if (id < numberOfTriangles())
        return 3;
    return 4;
}

inline int ParallelMesh::getFaceTag(int id) const {
    if (id < numberOfTriangles())
        return getTriangleTag(id);
    id -= numberOfTriangles();
    return getQuadTag(id);
}














