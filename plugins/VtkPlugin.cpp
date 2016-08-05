#include "VolumeMeshInterface.h"
#include <VtkUnstructuredWriter.h>
#include "ParfaitVizualizer.h"
#include "VtkPlugin.h"

class VtkVizualizer : public ParfaitVizualizer {
private:
    const Parfait::VolumeMeshInterface* mesh;
public:
    VtkVizualizer(const Parfait::VolumeMeshInterface* mesh) : mesh(mesh){}

    virtual void writeBinary(std::string name) const override {
        Parfait::VtkUnstructuredWriter writer(name, *mesh);
        writer.writeBinary();
    }
};

ParfaitVizualizer* registerMesh(const Parfait::VolumeMeshInterface* mesh){
    printf("Creating a new VtkVizualizer...\n");
    return new VtkVizualizer(mesh);
}
