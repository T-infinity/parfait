#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include "NodeCenteredRedistributor.h"
#include "ComponentPlacer.h"
#include "PartitionableMesh.h"

namespace Parfait{
  inline PreProcessor::PreProcessor(std::string xml_input_filename)
          :config(xml_input_filename)
  {
      setUpGridInfo(xml_input_filename);
  }


  inline std::vector<int> PreProcessor::calculateNewPartitioning(std::shared_ptr<MeshBasicParallel> mesh){
      PartitionableMesh partitionableMesh(mesh);

      Partitioner<decltype(partitionableMesh)> partitioner(partitionableMesh);
      return partitioner.generatePartVector();
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
      auto part = calculateNewPartitioning(mesh);
      if(MessagePasser::Rank() == 0)
          printf("Redistribute according to new partitioning:\n");
      ParallelMeshReDistributor distributor(mesh,part);
      return distributor.redistribute();
  }
}
