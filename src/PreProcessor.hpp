#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include <PartitionableMesh.h>
#include <ParallelPartitionableMesh.h>
#include <ParallelNodeToNodeBuilder.h>
#include "ParmetisWrapper.h"
#include "NodeCenteredRedistributor.h"
#include "timing.h"

namespace Parfait{
  inline PreProcessor::PreProcessor(std::string xml_input_filename)
          :config(xml_input_filename)
  {
      setUpGridInfo(xml_input_filename);
  }

  inline void PreProcessor::setUpGridInfo(std::string xml_input_filename){
      for(int i:range(config.numberOfGrids())){
          gridNames.push_back(config.getFilename(i));
          isBigEndian.push_back(config.isBigEndian(i));
          mapbcVector.push_back(config.getMapbcObject(i));
          grid_initial_positions.push_back(config.getMotionMatrix(i));
      }
  }

  inline std::shared_ptr<MeshBasicParallel> PreProcessor::createMesh(){
      auto before_reading = Now();
      Parfait::ParallelMeshReader reader(gridNames,isBigEndian);
      auto mesh = reader.distributeGridsEvenly();
      auto after_reading = Now();
      Parfait::ParallelPartitionableMesh partitionableMesh(mesh);
      Parfait::ParallelNodeToNodeBuilder<decltype(partitionableMesh)> n2n_builder(partitionableMesh);
      auto n2n = n2n_builder.buildNodeToNodeConnectivity();
      auto after_building_node_to_node = Now();
      auto part = Parfait::Partitioner::generatePartVector(n2n);
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
      //return distributor.redistribute();
  }
}
