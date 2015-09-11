#ifndef PARMETIS_PREPPER_H
#define PARMETIS_PREPPER_H
#include <vector>
#include <memory>
#include <MeshBasicParallel.h>

namespace Parfait {
  template <class Mesh>
    class Partitioner {
        template<class T> using vector = std::vector<T>;
    public:
        Partitioner() = delete;
        Partitioner(const Mesh & mesh_in);

        vector<int> generatePartVector();
        vector<vector<int>> local_connectivity;
    private:
        const Mesh& mesh;

        vector<long> buildProcNodeMap();
    };

}
#include "Partitioner.hpp"
#endif
