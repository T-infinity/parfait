#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>

namespace Parfait {
    class Partitioner {
        template<class T> using vector = std::vector<T>;
    public:
        Partitioner() = delete;
        Partitioner(ParallelImportedUgrid &mesh_in);

        vector<int> generatePartVector();
        vector<vector<long>> connectivity;
    private:
        ParallelImportedUgrid &mesh;
        vector<long> procNodeMap;

        void buildProcNodeMap();
    };

}
#include "Partitioner.hpp"
#endif
