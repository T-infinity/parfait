//
// Created by cdruyorj on 8/20/15.
//

#include "parfait_preprocessor.h"
#include "PreProcessor.h"

class PersistentPreProcessorObject{
public:
    PersistentPreProcessorObject(const char*filename)
            :preprocessor(filename),
             mesh(preprocessor.createFun3DMesh()){}
    Parfait::PreProcessor preprocessor;
    Parfait::Fun3DMesh mesh;
};

PersistentPreProcessorObject* stuff = nullptr;

void throwIfNotInitialized(){
    if(nullptr == stuff)
        throw std::logic_error("Must initialize preprocessor before using it...");
}


void parfait_create_grid_system(const char *filename) {
    stuff = new PersistentPreProcessorObject(filename);
}

void parfait_cleanup() {
    if(nullptr != stuff)
        delete stuff;
    stuff = nullptr;
}

int parfait_preprocessor_number_of_local_nodes() {
    throwIfNotInitialized();
    return stuff->mesh.numberOfNodes();
}
