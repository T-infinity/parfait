#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include <PartitionableMesh.h>
#include <ParallelPartitionableMesh.h>
#include <ParallelNodeToNodeBuilder.h>
#include "NodeCenteredRedistributor.h"
#include "timing.h"

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
      Parfait::ParallelMeshReader reader(grid_names,is_big_endian);
      auto mesh = reader.distributeGridsEvenly();
      auto after_reading = Now();
      Parfait::ParallelPartitionableMesh partitionableMesh(mesh);
      Parfait::ParallelNodeToNodeBuilder<decltype(partitionableMesh)> n2n_builder(partitionableMesh);
      auto n2n = n2n_builder.buildNodeToNodeConnectivity();
      auto after_building_node_to_node = Now();
      std::shared_ptr<Parfait::Partitioner> partitioner;
#ifdef PARFAIT_WITH_PARMETIS
      partitioner = std::make_shared<Parfait::ParmetisPartitioner>();
#else
      partitioner = std::make_shared<Parfait::ErrorPartitioner>();
#endif
      auto part = partitioner->generatePartVector(n2n);
      auto after_parmetis = Now();
      ParallelMeshReDistributor distributor(mesh,part);
      auto distributed = distributor.redistribute();
      auto after_redistributing = Now();

      if(0 == MessagePasser::Rank()) {
          printf("\nTime to read & naive partition: ");
          printReadableElapsedTime(before_reading, after_reading);
          printf("\nTime to make n2n: ");
          printReadableElapsedTime(after_reading, after_building_node_to_node);
          printf("\nTime to get part vector: ");
          printReadableElapsedTime(after_building_node_to_node, after_parmetis);
          printf("\nTime to redistribute accordingly: ");
          printReadableElapsedTime(after_parmetis, after_redistributing);
      }
      return distributed;
  }
}
