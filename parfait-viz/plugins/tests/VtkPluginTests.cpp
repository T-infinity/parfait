#include "catch.hpp"
#include <dlfcn.h>
#include <VolumeMeshInterface.h>
#include <CartBlock.h>
#include <VolumeMeshAdapter.h>
#include <ParfaitVizualizer.h>
#include <VizPluginLoader.h>

#ifdef __APPLE__
std::string dynamic_lib_ext = "dylib";
#else
std::string dynamic_lib_ext = "so";
#endif


TEST_CASE("VtkPlugin Can Load"){

    Parfait::CartBlock mesh({0,0,0},{1,1,1}, 5,5,8);
    Parfait::VolumeMeshAdapter adapter(mesh);
    printf("Writing binary\n");

    std::string lib_location = "../libParfaitVtkPlugin." + dynamic_lib_ext;
    auto plugin = Parfait::VizPlugin(lib_location);

    auto writer = plugin.registerMesh(&adapter);
    writer->writeBinary("vizualizes-cart-mesh");
}

