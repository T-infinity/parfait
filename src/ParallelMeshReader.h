#ifndef PARFAIT_PARALLELMESHREADER_H
#define PARFAIT_PARALLELMESHREADER_H

#include <string>
namespace Parfait{
    class ParallelMeshReader{
    public:
        static ImportedUgrid read(std::string fileName);
    private:
        ParallelMeshReader(std::string configurationFile);
        ConfigurationReader configurationReader;
        ImportedUgrid mesh;
        ImportedUgrid build();

        void distributeAllNodes();

        long getTotalNumberOfNodes();
    };

}
#include "ParallelMeshReader.hpp"
#endif
