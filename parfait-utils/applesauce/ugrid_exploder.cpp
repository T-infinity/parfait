#include <ExplodedMesh.h>
#include "input.h"
#include <ImportedUgridFactory.h>
#include <VtkGenericWriter.h>

using namespace Parfait;
int main(int argc,char* argv[]){
    Input input(argc,argv);

    if(input.isBigEndian())
        printf("Reading: %s as big endian\n",input.gridName().c_str());
    else
        printf("Reading: %s as little endian\n",input.gridName().c_str());

    auto ugrid = ImportedUgridFactory::readUgrid(input.gridName());

    printf("Creating: %s.vtu\n",input.projectName().c_str());
    ExplodedMesh explodedMesh(ugrid);
    VtkWriter<ExplodedMesh> writer(explodedMesh,input.projectName());
    writer.writeAscii();

    return 0;
}
