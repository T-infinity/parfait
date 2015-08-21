#ifndef PARFAIT_PARALLELMESHREADER_HPP
#define PARFAIT_PARALLELMESHREADER_HPP

#include "UgridReader.h"

namespace Parfait {

    inline ParallelMeshReader::ParallelMeshReader(std::string configurationFile)
            : configurationReader(configurationFile), mesh({},{},{},{},{},{},{},{},{},{},{}) { }

inline ImportedUgrid ParallelMeshReader::read(std::string fileName) {
    ParallelMeshReader reader(fileName);
    return reader.build();
}

inline ImportedUgrid ParallelMeshReader::build() {
    distributeAllNodes();
    return mesh;
}

inline void ParallelMeshReader::distributeAllNodes() {
    long totalNumberOfNodes = getTotalNumberOfNodes();
}

inline long ParallelMeshReader::getTotalNumberOfNodes() {
    for(int gridId = 0; configurationReader.numberOfGrids(); gridId++){
        UgridReader ugridReader(configurationReader.getFilename(gridId));
    }
}

}
#endif
