#pragma once

#ifdef __cplusplus
extern "C" {
#endif

namespace Parfait {
  class VolumeMeshInterface;
}
class ParfaitVizualizer;

ParfaitVizualizer* registerMesh(const Parfait::VolumeMeshInterface* mesh);

#ifdef __cplusplus
};
#endif

