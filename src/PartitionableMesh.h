#ifndef PARFAIT_PARTITIONABLEMESH_H
#define PARFAIT_PARTITIONABLEMESH_H

#include <vector>
#include <unordered_set>
namespace Parfait {
    class PartitionableMesh{
        private:
        int number_of_nodes;
        std::vector<int>& tets;
        std::vector<int>& pyramids;
        std::vector<int>& prisms;
        std::vector<int>& hexes;
        std::vector<long>& globalNodeIds;
        std::vector<int>& nodeOwnershipDegree;
        public:
        PartitionableMesh(std::shared_ptr<MeshBasicParallel> mesh) :
        tets(mesh->connectivity->tets),
                pyramids(mesh->connectivity->pyramids),
                prisms(mesh->connectivity->prisms),
                hexes(mesh->connectivity->hexes),
                globalNodeIds(mesh->metaData->globalNodeIds),
                nodeOwnershipDegree(mesh->metaData->nodeOwnershipDegree){
            std::unordered_set<int> nodes;
            for(int id : tets)
                nodes.insert(id);
            for(int id : pyramids)
                nodes.insert(id);
            for(int id : prisms)
                nodes.insert(id);
            for(int id : hexes)
                nodes.insert(id);
            number_of_nodes = nodes.size();
        }
        int numberOfNodes() const {
            return number_of_nodes;
        }
        long getGlobalNodeId(int localId) const {
            return globalNodeIds[localId];
        }
        bool doOwnLocalNode(int localId) const {
            return nodeOwnershipDegree[localId] == 0;
        }
        int numberOfTets()const {
            return tets.size() / 4;
        }
        int numberOfPyramids()const {
            return pyramids.size() / 5;
        }
        int numberOfPrisms()const {
            return prisms.size() / 6;
        }
        int numberOfHexes()const {
            return hexes.size() / 8;
        }
        const int* getTet(int cellId) const {
            return &tets[4*cellId];
        }
        const int* getPyramid(int cellId) const {
            return &pyramids[5*cellId];
        }
        const int* getPrism(int cellId) const {
            return &prisms[6*cellId];
        }
        const int* getHex(int cellId) const {
            return &hexes[8*cellId];
        }
    };
}

#endif
