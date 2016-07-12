#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include <PartitionableMesh.h>
#include <ParallelPartitionableMesh.h>
#include <ParallelNodeToNodeBuilder.h>
#include <thread>
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
      auto mesh = getDistributedMesh(grid_names,is_big_endian);
      if(MessagePasser::Rank() < 2){
          std::this_thread::sleep_for(std::chrono::seconds(2));
          printf("Rank %i has %i nodes %i cells %i surface faces\n",MessagePasser::Rank(),
                 (int)mesh->metaData->xyz.size()/3, mesh->connectivity->numberOfCells(),
                    mesh->connectivity->numberOfFaces());
      }
      auto after_reading = Now();
      Parfait::ParallelNodeToNodeBuilder<Parfait::ParallelMesh> n2n_builder(*mesh.get());
      if(MessagePasser::Rank() == 0) printf("Building node to node graph\n");
      auto n2n = n2n_builder.buildNodeToNodeConnectivity();
      auto after_building_node_to_node = Now();
      if(MessagePasser::Rank() == 0) printf("Calling partitioner\n");
      std::shared_ptr<Parfait::Partitioner> partitioner;
#ifdef PARFAIT_WITH_PARMETIS
      partitioner = std::make_shared<Parfait::ParmetisPartitioner>();
#else
      partitioner = std::make_shared<Parfait::ErrorPartitioner>();
#endif
      auto part = partitioner->generatePartVector(n2n);
      auto after_parmetis = Now();
      if(MessagePasser::Rank() == 0) printf("Redistributing according to part vector\n");
      NodeBasedRedistributor distributor(mesh,part);
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

    std::shared_ptr<ParallelMesh> PreProcessor::getDistributedMesh(std::vector<std::string>& grid_names,
                                                                   std::vector<bool>& is_big_endian) {
        auto reader = std::make_shared<Parfait::ParallelMeshReader>(grid_names,is_big_endian);
        return reader->distributeGridsEvenly();
    }
}
