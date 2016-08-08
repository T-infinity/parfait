#include "VizPluginLoader.h"

namespace Parfait {

    VizPlugin::VizPlugin(std::string libraryName)
    :registerMesh_fptr(extractRegisterMesh(libraryName))
    { }

    const VizPlugin::registerMesh_fptr_type VizPlugin::extractRegisterMesh(std::string libraryName) {
        auto handle = dlopen(libraryName.c_str(), RTLD_LAZY);
        if(not handle) {
            FILE* f = fopen("derp.dat","w");
            fprintf(f,"Herp\n");
            fclose(f);
            auto err = dlerror();
            printf("%s\n",err);
            throw std::logic_error("Could not load plugin: " + libraryName);
        }
        dlerror();
        auto apple = (ParfaitVizualizer*(*)(Parfait::VolumeMeshInterface*))dlsym(handle, "registerMesh");
        auto err = dlerror();
        if(err)
            printf("error: %s\n",err);

        return apple;
    }

    ParfaitVizualizer *VizPlugin::registerMesh(VolumeMeshInterface *mesh) {
        return registerMesh_fptr(mesh);
    }
}
