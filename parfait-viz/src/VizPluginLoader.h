#pragma once
#include <string>
#include "ParfaitVizualizer.h"
#include "VolumeMeshInterface.h"
#include <dlfcn.h>
#include <stdexcept>

namespace Parfait{

    class VizPlugin {
    public:
        VizPlugin(std::string libraryName);
        ParfaitVizualizer* registerMesh(VolumeMeshInterface* mesh);

    private:
        typedef ParfaitVizualizer* (*registerMesh_fptr_type)(Parfait::VolumeMeshInterface*);
        const registerMesh_fptr_type registerMesh_fptr;
        const registerMesh_fptr_type extractRegisterMesh(std::string libraryName);
    };
}
