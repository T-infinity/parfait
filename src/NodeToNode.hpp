#include "VectorTools.h"

template<typename MeshType>
std::vector <std::vector<long>> Parfait::NodeToNodeBuilder<MeshType>::buildNodeToNodeConnectivity() {
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
        if(left < node_to_node.size())
            insertUnique(node_to_node[left],mesh.getGlobalNodeId(right));
        if(right < node_to_node.size())
            insertUnique(node_to_node[right],mesh.getGlobalNodeId(left));
    }
}

template <typename MeshType>
std::vector<std::vector<long>> Parfait::NodeToNodeBuilder<MeshType>::returnSetsAsVectors(){
    std::vector <std::vector<long>> n2n;
    for (auto &set:node_to_node)
        n2n.push_back(std::vector<long>(set.begin(), set.end()));
    return n2n;
}
