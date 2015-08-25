#include "VectorTools.h"

template<typename MeshType>
std::vector <std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::buildNodeToNodeConnectivity() {
    for (int cell_id = 0; cell_id < mesh.numberOfCells(); cell_id++)
        for (int face_id = 0; face_id < mesh.numberOfFacesInCell(cell_id); face_id++)
            processFace(cell_id,face_id);

    return returnSetsAsVectors();
}

template <typename MeshType>
void Parfait::NodeToNodeBuilder<MeshType>::processFace(int cell_id,int face_id){
    auto face = mesh.getNodesInCellFace(cell_id,face_id);
    for (int i = 0; i < face.size(); i++) {
        int left = face[i];
        int right = face[(i + 1) % face.size()];
        // TODO: convert to local id's and save only if you own it
        //node_to_node[left].insert(right);
        insertUnique(node_to_node[left],right);
        //node_to_node[right].insert(left);
        insertUnique(node_to_node[right],left);
    }
}

template <typename MeshType>
std::vector<std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::returnSetsAsVectors(){
    std::vector <std::vector<int>> n2n;
    for (auto &set:node_to_node)
        n2n.push_back(std::vector<int>(set.begin(), set.end()));
    return n2n;
}
