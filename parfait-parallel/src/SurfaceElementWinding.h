#pragma once
#include <ParallelMesh.h>
#include <memory>
#include <vector>

bool areSurfaceElementsWoundOut(const std::shared_ptr<Parfait::ParallelMesh>& mesh,
                                const std::vector<int>& surface_neighbors);

void windAllSurfaceElementsOut(std::shared_ptr<MessagePasser> mp,
                               std::shared_ptr<Parfait::ParallelMesh>& mesh,
                               const std::vector<int>& surface_neighbors);