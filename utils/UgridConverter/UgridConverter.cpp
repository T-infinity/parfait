#include "../../parfait/ImportedUgridFactory.h"
#include "../../parfait/UgridWriter.h"
#include <VtkUnstructuredWriter.h>
#include <VtkUnstructuredSurfaceWriter.h>
#include <iostream>
#include <memory>
#include <json.hpp>
#include "CommandLineParser.h"
#include "../../parfait/ExplodedMesh.h"


std::string stripExtension(std::string in){
    return in.substr(0,in.rfind(".ugrid"));
}

std::shared_ptr<ImportedUgrid> readInputGrid(std::string filename, std::string file_type){
    if(file_type == "ascii") {
        printf("\nReading file <%s> as ascii.\n", filename.c_str());
        return std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgridAscii(filename)
        );
    } else if (file_type == "lb8") {
        return std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgrid(filename));
    } else if (file_type == "b8") {
        return std::make_shared<ImportedUgrid>(
            Parfait::ImportedUgridFactory::readUgrid(filename, true));
    } else {
        throw std::logic_error("\nInvalid file type for reading\n\n");
    }
}

void writeOutputFile(std::shared_ptr<ImportedUgrid> ugrid, std::string filename, std::string file_type){
    if(file_type == "binary"){
        printf("Exporting <%s> as binary.", filename.c_str());
        Parfait::UgridWriter::writeImportedUgrid(*ugrid.get(), filename + ".lb8.ugrid", false);
    } else if(file_type == "vtk") {
        printf("Exporting <%s> as vtu.", filename.c_str());
        Parfait::VtkUnstructuredWriter writer(filename, *ugrid.get());
        writer.writeBinary();
    } else if(file_type == "vtk-surface"){
        printf("Exporting <%s> surfaces as vtu.", filename.c_str());
        Parfait::VtkUnstructuredSurfaceWriter writer(filename + ".surface", *ugrid.get());
        std::vector<int> bc_tag;
        for(auto t : ugrid->triangleTags)
            bc_tag.push_back(t);
        for(auto t : ugrid->quadTags)
            bc_tag.push_back(t);
        writer.addCellData("boundary-tag", bc_tag.data(), 1);
        writer.writeBinary();
    } else if(file_type == "vtk-exploded") {
        printf("Exporting <%s> as exploded vtu", filename.c_str());
        Parfait::ExplodedMesh explodedMesh(*ugrid.get());;
        Parfait::VtkUnstructuredWriter writer(filename + ".exploded", explodedMesh);
        writer.writeBinary();
    } else {
        std::cout << "Request <" << file_type << "> is unknown" << std::endl;
    }
}


int main(int argc, char* argv[]) {

    std::vector<std::string> command_line;
    for(int i = 0; i < argc; i++){
        command_line.push_back(argv[i]);
    }

    auto config = getConfigFromCommandLine(command_line);

    std::string filename = config["input"]["file"];
    std::string input_type = config["input"]["type"];

    auto ugrid = readInputGrid(config["input"]["file"], config["input"]["type"]);

    auto base = stripExtension(filename);

    writeOutputFile(ugrid, config["output"]["file"], config["output"]["type"]);

    std::cout << std::endl;
    return 0;
}
