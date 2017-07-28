#include "../../parfait/ImportedUgridFactory.h"
#include "../../parfait/UgridWriter.h"
#include <VtkUnstructuredWriter.h>
#include <VtkUnstructuredSurfaceWriter.h>
#include <iostream>
#include <memory>
#include "../../parfait/ExplodedMesh.h"


std::string stripExtension(std::string in){
    return in.substr(0,in.rfind(".ugrid"));
}

int main(int argc, char* argv[]) {

    std::string filename = argv[1];

    std::shared_ptr<ImportedUgrid> ugrid;
    std::string s;

    std::cout << "Enter file type: (ascii, binary, big-endian-binary)" << std::endl;
    std::getline(std::cin, s);
    if(s == "ascii") {
        printf("\nReading file <%s> as ascii.\n", filename.c_str());
        ugrid = std::make_shared<ImportedUgrid>(
                Parfait::ImportedUgridFactory::readUgridAscii(filename)
        );
    } else if (s == "binary") {
        ugrid = std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgrid(filename));
    } else if (s == "big-endian-binary") {
        ugrid = std::make_shared<ImportedUgrid>(
                Parfait::ImportedUgridFactory::readUgrid(filename, true));
    } else {
        throw std::logic_error("\nInvalid file type for reading\n\n");
    }

    auto base = stripExtension(filename);
    Parfait::MapbcReader reader(base+".mapbc");


    printf("What is the desired output? (binary, vtk, vtk-surface, vtk-exploded)  \n");
    std::getline(std::cin, s);
    if(s == "binary"){
        printf("Exporting <%s> as binary.", filename.c_str());
        Parfait::UgridWriter::writeImportedUgrid(*ugrid.get(), filename + ".lb8.ugrid", false);
    } else if(s == "vtk") {
        printf("Exporting <%s> as vtu.", filename.c_str());
        Parfait::VtkUnstructuredWriter writer(filename, *ugrid.get());
        writer.writeBinary();
    } else if(s == "vtk-surface"){
        printf("Exporting <%s> surfaces as vtu.", filename.c_str());
        Parfait::VtkUnstructuredSurfaceWriter writer(filename + ".surface", *ugrid.get());
        std::vector<int> bc_tag;
        for(auto t : ugrid->triangleTags)
            bc_tag.push_back(t);
        for(auto t : ugrid->quadTags)
            bc_tag.push_back(t);
        writer.addCellData("boundary-tag", bc_tag.data(), 1);
        std::vector<int> boundary_condition;
        for(auto t : ugrid->triangleTags)
            boundary_condition.push_back(reader.boundaryCondition(t));
        for(auto t : ugrid->quadTags)
            boundary_condition.push_back(reader.boundaryCondition(t));
        writer.addCellData("boundary-condition", boundary_condition.data(), 1);
        writer.writeBinary();
    } else if(s == "vtk-exploded") {
        printf("Exporting <%s> as exploded vtu", filename.c_str());
        Parfait::ExplodedMesh explodedMesh(*ugrid.get());;
        Parfait::VtkUnstructuredWriter writer(filename + ".exploded", explodedMesh);
        writer.writeBinary();
    } else {
        std::cout << "Request <" << s << "> is unknown" << std::endl;
    }

    std::cout << std::endl;
    return 0;
}
