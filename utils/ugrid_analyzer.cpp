#include "imported_ugrid_factory.h"
#include "mesh_analyzer.h"
#include "input.h"
#include <string>
#include <iostream>

int main(int argc,char* argv[]){
    Input input(argc,argv);
   
    if(input.isBigEndian())
        printf("Reading: %s as big endian\n",input.gridName().c_str());
    else
        printf("Reading: %s as little endian\n",input.gridName().c_str());

    auto ugrid = ImportedUgridFactory::readUgrid(input.gridName());

    MeshAnalyzer<ImportedUgrid> analyzer(ugrid);
    analyzer.printStats();
    analyzer.throwIfBadIndexFound();
    
    return 0;
}



