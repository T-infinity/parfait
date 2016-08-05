#include "catch.hpp"
#include <dlfcn.h>


TEST_CASE("VtkPlugin Can Load"){

    std::string lib_location = ".libParfaitVtkPlugin.so";
    auto handle = dlopen(lib_location.c_str(), RTLD_LAZY);
    if(not handle) {
        throw std::logic_error("Could not load plugin: " + lib_location);
    }
}

