#include "catch.hpp"
#include <dlfcn.h>
#include <VolumeMeshInterface.h>
#include <CartBlock.h>
#include <VolumeMeshAdapter.h>
#include <ParfaitVizualizer.h>

TEST_CASE("VtkPlugin Can Load"){

    std::string lib_location = "../../../build/plugins/libParfaitVtkPlugin.so";
    auto handle = dlopen(lib_location.c_str(), RTLD_LAZY);
    if(not handle) {
        FILE* f = fopen("derp.dat","w");
        fprintf(f,"Herp\n");
        fclose(f);
        auto err = dlerror();
        printf("%s\n",err);
        throw std::logic_error("Could not load plugin: " + lib_location);
    }
    dlerror();
    ParfaitVizualizer* (*registerMesh)(Parfait::VolumeMeshInterface*) = (ParfaitVizualizer*(*)(Parfait::VolumeMeshInterface*))dlsym(handle, "registerMesh");
    auto err = dlerror();
    if(err)
        printf("error: %s\n",err);

    Parfait::CartBlock mesh({0,0,0},{1,1,1}, 5,5,8);
    Parfait::VolumeMeshAdapter adapter(mesh);
    printf("Wriging binary");
    auto viz = registerMesh(&adapter);
    viz->writeBinary("vizualizes-cart-mesh");
}

