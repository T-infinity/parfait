#include "vtk_surface_writer.h"
#include "imported_ugrid_factory.h"
#include "mapbc_reader.h"
#include "input.h"
#include <string>
#include <iostream>
using namespace Parfait;
int main(int argc,char* argv[]){
    Input input(argc,argv);
   
    if(input.isBigEndian())
        printf("Reading: %s as big endian\n",input.gridName().c_str());
    else
        printf("Reading: %s as little endian\n",input.gridName().c_str());

    auto ugrid = ImportedUgridFactory::readUgrid(input.gridName(),input.isBigEndian());
    vector<int> tags,bcs;
    for(int i=0;i<ugrid.numberOfBoundaryFaces();i++){
        tags.push_back(ugrid.getBoundaryTag(i));
        bcs.push_back(ugrid.getBoundaryCondition(i));
    }

    printf("Creating: %s_surface.vtu\n",input.projectName().c_str());
    VtkSurfaceWriter<ImportedUgrid> writer(ugrid,input.projectName()+"_surface");
    writer.addScalarField(false,"Boundary Tags",tags.data());
    writer.addScalarField(false,"Boundary Conditions",bcs.data());
    writer.writeAscii();
    
    return 0;
}



