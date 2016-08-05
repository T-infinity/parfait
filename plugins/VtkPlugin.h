#pragma once

#ifdef __cplusplus
extern "C" {
#endif
class VolumeMeshInterface;

void registerMesh(const VolumeMeshInterface* mesh);
void writeBinary(const char* name);

#ifdef __cplusplus
};
#endif

