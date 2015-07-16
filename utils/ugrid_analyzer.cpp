#include "ImportedUgridFactory.h"
#include "mesh_analyzer.h"
#include "input.h"
#include "UgridMeshAnalyzer.h"
#include <string>
#include <iostream>

int main(int argc,char* argv[]){
    Input input(argc,argv);
   
    if(input.isBigEndian())
        printf("Reading: %s as big endian\n",input.gridName().c_str());
    else
        printf("Reading: %s as little endian\n",input.gridName().c_str());

    auto ugrid = Parfait::ImportedUgridFactory::readUgrid(input.gridName());

    UgridMeshAnalyzer analyzer(ugrid);
    analyzer.printStats();
    analyzer.printNegativeVolumeStatistics();
    analyzer.throwIfBadIndexFound();
    return 0;
}



