#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>

namespace Parfait {
    template<class MeshType>
    class Partitioner {
        template<class T> using vector = std::vector<T>;
    public:
        Partitioner() = delete;
        Partitioner(MeshType &mesh_in);

        vector<int> generatePartVector();
        vector<vector<int> > connectivity;
    private:
        MeshType &mesh;
        vector<int> procNodeMap;

        void buildProcNodeMap();
    };

}
#include "Partitioner.hpp"
#endif
