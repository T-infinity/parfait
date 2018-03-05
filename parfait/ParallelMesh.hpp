
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include <MessagePasser/MessagePasser.h>

namespace Parfait {
inline ParallelMeshBuilder::ParallelMeshBuilder(MessagePasser mp) :
    messagePasser(mp), data(std::make_shared<ParallelMeshData>()) {}

inline ParallelMeshBuilder::ParallelMeshBuilder(MessagePasser mp,
                                                ParallelMesh mesh) :
    messagePasser(mp), data(mesh.data) {
}

inline std::shared_ptr<ParallelMesh> ParallelMeshBuilder::exportMesh() {
    return std::make_shared<Parfait::ParallelMesh>(messagePasser, *this);
}

inline int ParallelMesh::countOwnedNodes() const {
    int rank = messagePasser.Rank();
    return int(std::count(data->nodeOwnerRank.begin(),
                          data->nodeOwnerRank.end(), rank));
}

inline ParallelMesh::ParallelMesh(MessagePasser messagePasser,
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
    return data->nodeOwnerRank[id] != messagePasser.Rank();
}

inline int ParallelMesh::numberOfTets() const {
    return int(data->tets.size() / 4);
}

inline const int *ParallelMesh::getTet(int id) const {
    return &(data->tets[4 * id]);
}

inline int ParallelMesh::numberOfPyramids() const {
    return int(data->pyramids.size() / 5);
}

inline const int *ParallelMesh::getPyramid(int id) const {
    return &(data->pyramids[5 * id]);
}

inline int ParallelMesh::numberOfPrisms() const {
    return int(data->prisms.size() / 6);
}

inline const int *ParallelMesh::getPrism(int id) const {
    return &(data->prisms[6 * id]);
}

inline int ParallelMesh::numberOfHexes() const {
    return int(data->hexs.size() / 8);
}

inline const int *ParallelMesh::getHex(int id) const {
    return &(data->hexs[8 * id]);
}
inline int ParallelMesh::numberOfTriangles() const {
    return int(data->triangles.size() / 3);
}
inline const int *ParallelMesh::getTriangle(int id) const {
    return &(data->triangles[3 * id]);
}
inline int ParallelMesh::numberOfQuads() const {
    return int(data->quads.size() / 4);
}
inline const int *ParallelMesh::getQuad(int id) const {
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

inline const int *ParallelMesh::getCell(int id) const {
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

inline void ParallelMesh::getNodesInCell(int id, int *cell) const {
    auto cell_ptr = getCell(id);
    int n = numberOfNodesInCell(id);
    for (int i = 0; i < n; i++)
        cell[i] = cell_ptr[i];
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

inline const int *ParallelMesh::getFace(int id) const {
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
}













