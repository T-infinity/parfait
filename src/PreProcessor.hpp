#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include <PartitionableMesh.h>
#include <ParallelPartitionableMesh.h>
#include <ParallelNodeToNodeBuilder.h>
#include "ParmetisWrapper.h"
#include "NodeCenteredRedistributor.h"

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
      Parfait::ParallelMeshReader reader(gridNames,isBigEndian);
      auto mesh = reader.distributeGridsEvenly();
      Parfait::ParallelPartitionableMesh partitionableMesh(mesh);
      Parfait::ParallelNodeToNodeBuilder<decltype(partitionableMesh)> n2n_builder(partitionableMesh);
      auto n2n = n2n_builder.buildNodeToNodeConnectivity();
      auto part = Parfait::Partitioner::generatePartVector(n2n);
      ParallelMeshReDistributor distributor(mesh,part);
      return distributor.redistribute();
  }
}
