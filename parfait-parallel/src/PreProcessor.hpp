#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include <ParallelNodeToNodeBuilder.h>
#include <thread>
#include "NodeCenteredRedistributor.h"
#include "Timing.h"
#include "SurfaceNeighbors.h"
#include "SurfaceElementWinding.h"

#ifdef PARFAIT_WITH_PARMETIS
#include "ParmetisPartitioner.h"
#endif

namespace Parfait{
  inline PreProcessor::PreProcessor(Configuration& configuration)
          :config(configuration)
  { }

  inline std::shared_ptr<ParallelMesh> PreProcessor::createMesh(){
      auto before_reading = Now();
      std::vector<std::string> grid_names;
      std::vector<bool> is_big_endian;
      for(int i=0;i<config.numberOfGrids();i++){
          grid_names.push_back(config.getFilename(i));
          is_big_endian.push_back(config.isBigEndian(i));
      }
      auto mesh = getDistributedMesh(grid_names,is_big_endian);
      auto after_reading = Now();
      Parfait::ParallelNodeToNodeBuilder<Parfait::ParallelMesh> n2n_builder(*mesh.get());
      auto n2n = n2n_builder.buildNodeToNodeConnectivity();
      auto after_building_node_to_node = Now();
      std::shared_ptr<Parfait::Partitioner> partitioner;

      partitioner = std::make_shared<Parfait::ParmetisPartitioner>();

      auto part = partitioner->generatePartVector(n2n);
      auto after_parmetis = Now();
      NodeBasedRedistributor distributor(mesh,part);
      auto distributed = distributor.redistribute();
      auto after_redistributing = Now();

      auto surface_neighbors = SurfaceNeighbors::buildSurfaceNeighbors(distributed);
      windAllSurfaceElementsOut(distributed, surface_neighbors);
      if(not areSurfaceElementsWoundOut(distributed, surface_neighbors))
          throw std::logic_error("\nSurface elements are not all wound out");

      return distributed;
  }

  inline std::shared_ptr<ParallelMesh> PreProcessor::getDistributedMesh(std::vector<std::string>& grid_names,
                                                                   std::vector<bool>& is_big_endian) {
      auto reader = std::make_shared<Parfait::ParallelMeshReader>(grid_names,is_big_endian);
      return reader->distributeGridsEvenly();
  }
}
