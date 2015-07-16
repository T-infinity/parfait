#ifndef PARFAIT_UGRIDANALYZER_H
#define PARFAIT_UGRIDANALYZER_H

#include <ImportedUgrid.h>

class UgridMeshAnalyzer {
public:
    UgridMeshAnalyzer(Parfait::ImportedUgrid &ugrid);
    void printStats();
    void throwIfBadIndexFound();
    void throwIfNegativeVolumeFound();
    void printNegativeVolumeStatistics();
private:
    Parfait::ImportedUgrid &ugrid;
};

#endif //PARFAIT_UGRIDANALYZER_H
